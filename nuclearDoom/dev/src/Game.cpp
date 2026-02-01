#include "stdafx.h"

#include <unordered_map>
#include <EASTL/variant.h>
#include "../../../skd/dev/src/App_GameState.h"

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

void Game::hitWallPix(float px, float py){
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
	ui = new UI(root);
	bulMan = new BulMan(this,&al);
	player = new Player(this,root);

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
}

void Game::update(double dt) {
	Super::update(dt);

	controls(dt);
	al.update(dt);
	tw.update(dt);
#ifdef PASTA_DEBUG
	im();	
#endif
}
/*
template <> void Pasta::JReflect::visit(std::vector<Vector2> & v, const char* name) {
	u32 arrSize = v.size()*2;
	if (visitArrayBegin(name, arrSize)) {
		if (isReadMode())
			if (v.size() < arrSize >> 1)
				v.resize(arrSize >> 1);
		for (u32 i = 0; i < arrSize; ++i) {
			visitIndexBegin(i);
			Vector2& vf = v[i>>1];
			if( 0==(i & 1) )
				visit(vf.x, nullptr);
			else
				visit(vf.y, nullptr);
			visitIndexEnd();
		}
	}
	visitArrayEnd(name);
}

static void visitEastl(Pasta::JReflect&jr,eastl::vector<Vector2>& v, const char* name) {
	u32 arrSize = v.size() * 2;
	if (jr.visitArrayBegin(name, arrSize)) {
		if (jr.isReadMode())
			if( v.size() < arrSize >>1)
				v.resize(arrSize >> 1);
		for (u32 i = 0; i < arrSize; ++i) {
			jr.visitIndexBegin(i);
			Vector2& vf = v[i >> 1];
			if (0 == (i & 1))
				jr.visit(vf.x, nullptr);
			else
				jr.visit(vf.y, nullptr);
			jr.visitIndexEnd();
		}
	}
	jr.visitArrayEnd(name);
}
*/


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