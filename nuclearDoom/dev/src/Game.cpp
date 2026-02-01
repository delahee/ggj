#include "stdafx.h"

#include <unordered_map>
#include <EASTL/variant.h>
#include "App_GameState.h"

#include "r2/Node.hpp"
#include "rd/ABitmap.hpp"
#include "rui/Canvas.hpp"
#include "r2/StaticBox.hpp"

#include "BulMan.hpp"
#include "Game.hpp"
#include "rd/JSerialize.hpp"
#include "rd/Garbage.hpp"
#include "Entity.hpp"
#include "r2/fx/Part.hpp"
#include "FX.hpp"
#include "Map.hpp"
#include "UI.hpp"

using namespace rd;
using namespace std::string_literals;

void Game::onFrag(){
	frags++;

}

bool Game::isWallPix(float px, float py){
	int cx = px / Cst::GRID;
	int cy = py / Cst::GRID;
	if (isWallGrid(cx,cy)) 
		return true;
	return false;
}

bool Game::isWallGrid(int cx, int cy){
	if (map->softWalls.get(cx, cy)) 
		return true;
	if (map->hardWalls.get(cx, cy))
		return true;
	return false;
}


void Game::explode(int atX, int atY, ProjData* proj) {
	if (!proj)return;
	if (proj->name != "rocket")
		return;

	auto& rnd = rd::Rand::get();
	for (int i = 0; i < 6; ++i) {
		auto gfx = r2::Graphics::fromPool(root);
		if (rnd.pc(50))
			gfx->color = r::Color::Yellow;
		else
			gfx->color = r::Color::Black;
		gfx->drawDisc(0.0f, 0.0f, 8);
		gfx->x = atX + rnd.dice(-16, 16);
		gfx->y = atY + rnd.dice(-16, 16);
		gfx->setUniformScale(rnd.fuzz(1.2, 0.5));
		gfx->trsDirty = true;

		if (rnd.pc(50))
			rs::Timer::delay(50, [=] { gfx->color = r::Color::White; });
		if (rnd.pc(50))
			rs::Timer::delay(100, [=] { gfx->color = r::Color::Black; });
		rs::Timer::delay(150, [=] { gfx->destroy(); });
	}
}


void Game::hitWallPix(float px, float py, ProjData*proj){
	int cx = px / Cst::GRID;
	int cy = py / Cst::GRID;
	map->softWalls.set(cx, cy, 0);

	std::string coordTag = std::to_string(cx) + "_" + std::to_string(cy);
	auto b = map->wallBatch->head;
	while (b) {
		if (b->vars.hasTag("soft") && b->vars.hasTag(coordTag.c_str())) {
			b->destroy();
			break;
		}
		b = b->next;
	}

	bloodsplash(px, py);
	explode(px, py,proj);
}

void Game::bloodsplash(int px, int py){
	rd::Rand& rand = Rand::get();
	for (int i = 0; i < 16; ++i) {

		int sz = rand.dice(1, 2);
		r2::Graphics* sp = r2::Graphics::rect(-sz * 0.5f, -sz * 0.5f, sz, sz, 0xff0000, 1.0f, root);
		auto p = new r2::fx::Part(sp, &al);
		p->x = px;
		p->y = py - rand.dice(4, 12);
		auto a = rand.angle();
		float speed = rand.diceF(0.9f, 1.1f);
		sp->rotation = rand.angle();
		p->dx = cos(a) * speed;
		p->dy = sin(a) * speed;
		p->frictX = p->frictY = 0.92f + rand.diceF(0, 0.02f);
		p->gy = 0.1f;
		p->setLife(p->getLife() * rand.diceF(0.9f, 1.1f));
		p->groundY = py + rand.dice(12, 24);
		p->useGround = true;
	}
}


void Game::screenshake(float dur, float dx, float dy){
	auto sc = scRoot->getScene();
	if (shakeDur < 0)shakeDur = 0;
	shakeDur += dur;
	shakeX = dx;
	shakeY = dy;
	ocamX = sc->cameraPos.x;
	ocamY = sc->cameraPos.y;
}

Game::~Game()
{
	if (ui) {
		ui->destroy();
		ui = 0;
	}
	dispose();
}

void Game::dispose(){
	scRoot->destroyAllChildren();
	
	Super::dispose();
}
void Game::endscreen(){
	rs::Timer::delay([=]() {
		auto app = App_GameState::me;
		auto sc = root->getScene();
		auto gfx = r2::Graphics::rect(-200, -200, 4000, 4000,0x0,1.0f,root);
		auto txt = r2::Text::fromPool(nullptr,"The end", root);
		enabled = false;
		sc->setPan(0,0);
		sc->syncViewMatrix();
		txt->centered();
		txt->x = Cst::W * 0.5f;
		txt->y = Cst::H * 0.5f;
	});
	//delete this;
}


static bool defeated = false;
void Game::defeat(){
	if (defeated)
		return;

}

void Game::intro(){
	

}

void Game::sfx(const char * name){
	rd::AudioMan::get().playFile(name);
}

void Game::beginGame(){
	
}

void Game::victory() {

	
}

void Game::hit() {
	
}

void Game::controls(double dt){
	player->controls(dt);
}

Game::Game(r2::Node* _root, r2::Scene* sc, rd::AgentList* parent) : Super(parent) {
	scRoot = _root;
	root = new r2::StaticBox(r2::Bounds::fromTLWH(0, 0, Cst::W, Cst::H), scRoot);

	Data::init();

	rd::AudioMan::get().init();

	map = new Map(root);
	
	bulMan = new BulMan(this,&al);
	player = new Player(this,root);
	ui = new UI(this, root);

	player->init("player");
	player->setGridPos(map->playerSpawn.x, map->playerSpawn.y);

	tw.create(sc, (rs::TVar) r2::Scene::VCamPosX, (map->playerSpawn.x * Cst::GRID) - scRoot->x * sc->getZoomX());
	tw.create(sc, (rs::TVar) r2::Scene::VCamPosY, (map->playerSpawn.y * Cst::GRID) - scRoot->y * sc->getZoomY());

	if (false) {
		auto e = new Entity(this, root);
		e->init("imp");
		e->setPixelPos(100, 100);
		nmies.push_back(e);
	}

	for(auto pos : map->impList){
		auto e = new Entity(this, root);
		e->init("imp");
		e->setGridPos(pos.x, pos.y);
		nmies.push_back(e);
	}

	for (auto pos : map->blobList) {
		auto e = new Entity(this, root);
		e->init("blob");
		e->setGridPos(pos.x, pos.y);
		nmies.push_back(e);
	}
	for (auto pos : map->bossList) {
		auto e = new Entity(this, root);
		e->init("boss");
		e->setGridPos(pos.x, pos.y);
		nmies.push_back(e);
	}

	ui->toFront();
}

void Game::freezeFrame(float dur ){
	if (freezeDur < 0)
		freezeDur = 0;
	freezeDur += dur;
}

void Game::update(double dt) {
	Super::update(dt);

	if (!enabled)
		return;

	float nbFr = dt / (1.0 / 60.0);
	auto sc = scRoot->getScene();
	bool wasShaking = isShaking();
	shakeDur -= dt;

	if (shakeDur > 0.0f) {
		sc->cameraPos.x = ocamX + shakeX;
		sc->cameraPos.y = ocamY + shakeY;
		shakeX = -shakeX;
		shakeY = -shakeY;
		sc->syncViewMatrix();
	}
	else {
		if (wasShaking && !isShaking()) {
			sc->cameraPos.x = ocamX;
			sc->cameraPos.y = ocamY;
			sc->syncViewMatrix();
		}
	}

	if (freezeDur >= 0) {
		freezeDur -= dt;
		//return;
	}

	controls(dt);

	if (rs::Input::isJustPressed(Pasta::Key::KB_R)) {
		endscreen();
		return;
	}

	//regular pan

#if 1
	bool isCameraTweened = tw.exists(sc, (rs::TVar)r2::Scene::VCamPosX);
	if (!wasShaking && !isShaking() && !isCameraTweened) {
		float destX = (player->getPos().x) - scRoot->x * sc->getZoomX();
		float destY = (player->getPos().y) - scRoot->y * sc->getZoomX();
		sc->cameraPos.x = r::Math::lerp(sc->cameraPos.x, destX, pow(0.8f, nbFr));
		sc->cameraPos.y = r::Math::lerp(sc->cameraPos.y, destY, pow(0.8f, nbFr));
	}
#endif
	

	al.update(dt);
	tw.update(dt);
#ifdef PASTA_DEBUG
	im();	
#endif
}

bool Game::im(){
	using namespace ImGui;
	static bool opened = true;
	if (Begin("Game", &opened)) {
		if (TreeNode("player")) {
			player->im();
			TreePop();
		}
		int idx = 0;
		for (auto e : nmies){
			if (TreeNode("Nmy"s + std::to_string(idx))) {
				e->im();
				TreePop();
			}
			idx++;
		}
		idx = 0;
		
		for (; idx < bulMan->nbActive;++idx) {
			if (TreeNode("Bul "s + std::to_string(idx))) {
				bulMan->im(idx);
				TreePop();
			}
			idx++;
		}

		if (TreeNode("asset test")) {
			if (Button("Boss Shoot")) {
				::Bullet b;
				b.sprName = "boss shoot";
				b.x = 200;
				b.y = 100;
				b.dy = 1;
				b.frictx = b.fricty = 1;
				bulMan->addBullet(b);
			}

			if (Button("Bullet")) {
				::Bullet b;
				b.sprName = "bullet";
				b.x = 250;
				b.y = 100;
				b.dy = 50;
				b.frictx = b.fricty = 1;
				bulMan->addBullet(b);
			}

			static int wallType = 1;
			SliderInt("wall type", &wallType,  1, 7);
			if (Button("Wall")) {
				map->wallBatch->destroyAllElements();
				for (int i = 0; i < 16; ++i) {
					std::string label = "wall"s + std::to_string(wallType);
					auto b = r2::BatchElem::fromLib(Data::assets,label.c_str(),map->wallBatch);
					b->x = 16 + i * b->width();
					b->y = 16;
				}
			}

			if (Button("Shake 2,0")) {
				screenshake(0.2, 2, 0);
			}
			if (Button("Shake 0,2")) {
				screenshake(0.2, 0, 2);
			}
			if (Button("Shake 2,2")) {
				screenshake(0.2, 2, 2);
			}

			if (Button("Blob")) {
				
				auto b = r2::Bitmap::fromLib(Data::assets, "blob", root);
				b->x = 64;
				b->y = 64;
				
			}
			TreePop();
		}

		auto sc = App_GameState::me->mainScene;
		vec2 mouseScreen = { (float)rs::Sys::mouseX, (float)rs::Sys::mouseY };
		vec2 mouseSc = sc->globalToLocal( r2::Lib::screenToGlobal(sc, mouseScreen) );
		vec2 mouseGame = scRoot->globalToLocal(mouseSc);
		Value("mouse abs", mouseScreen);
		Value("mouse scene", mouseSc);
		Value("mouse game", mouseGame);
		End();
	}
	return false;
}

vec2 Game::getGameMousePos()
{
	auto sc = App_GameState::me->mainScene;
	vec2 mouseScreen = { (float)rs::Sys::mouseX, (float)rs::Sys::mouseY };
	vec2 mouseSc = sc->globalToLocal(r2::Lib::screenToGlobal(sc, mouseScreen));
	vec2 mouseGame = scRoot->globalToLocal(mouseSc);
	return mouseGame;
}