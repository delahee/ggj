#include "stdafx.h"

#include "all.hpp"
#include "Game.hpp"
#include "Entity.hpp"
#include "BulMan.hpp"
#include "UserTypes.hpp"
#include "rd/Garbage.hpp"
#include "r2/fx/Part.hpp"

static bool inited = false;

static EntityData* dummy = 0;
static EntityData* imp = 0;
static EntityData* player = 0;


EntityData* EntityData::get(const char* name){
	if( Data::entities.find(name) == Data::entities.end())
		return dummy;
	return Data::entities[name];
}

std::vector<Entity*> Entity::ALL;

void Entity::init(const char* name){
	EntityData* data = EntityData::get(name);
	init(data);
}

void Entity::init(EntityData * _data) {
	data = _data;
	std::string label = data->sprName.empty() ? data->name : data->sprName;
	if (spr == 0){
		spr = rd::ABitmap::fromPool(Data::assets, label.c_str(), this);
	}else
		spr->set(Data::assets, label.c_str());
	spr->setCenterRatio(0.5, 0.5);
	name = data->name + "#" + std::to_string(uid);
	hp = data->hp;
	blinking = false;
	fadingOut= false;
	alpha = 1;
}

Entity::Entity(Game*g,r2::Node* parent) : r2::Node(parent){
	game = g;
	ALL.push_back(this);
}

Entity::~Entity() {
	rs::Std::remove(ALL,this);
	dispose();
}

void Entity::fire(int x, int y) {
	fire(x, y, Data::getProj("dummy"));
}

void Entity::muzzle(int toX, int toY, ProjData* proj) {
	auto ppos = getPixelPos();
	auto gfx = r2::Graphics::fromPool(parent);

	vec2 dir = vec2( toX - ppos.x, toY - ppos.y);
	dir = dir.getNormalizedSafeZero();
	dir *= 8;

	if( proj && proj->name=="plasma")
		gfx->color = r::Color::Blue;
	else 
		gfx->color = r::Color::Yellow;
	gfx->drawDisc(0.0f, 0.0f, 8);
	gfx->x = x + dir.x; 
	gfx->y = y + dir.y; 
	gfx->trsDirty = true;

	rs::Timer::delay(20, [=] { gfx->color = r::Color::White; });
	rs::Timer::delay(40, [=] { gfx->color = r::Color::Black; });
	rs::Timer::delay(60, [=] { gfx->destroy(); });
}

void Entity::fire(int toX, int toY, ProjData * proj) {
	auto ppos = getPixelPos();

	muzzle(toX, toY,proj);

	//recoil
	{
		vec2 dir = - vec2(toX - ppos.x, toY - ppos.y);
		dir = dir.getNormalizedSafeZero();
		dx += dir.x * 0.2;
		dy += dir.y * 0.2;
	}

	Bullet b;
	b.x = ppos.x;
	b.y = ppos.y;
	b.dmg = proj->dmg;
	b.sprName = proj->sprName;
	b.flags = proj->flags;
	b.proj = proj;

	float speed = proj->speed;

	vec2 dir = { toX - b.x, toY - b.y };
	dir = dir.getNormalizedSafeZero();
	b.dx = dir.x * speed;
	b.dy = dir.y * speed;

	if (!b.dx && !b.dy) {
		b.dy = 1;
	}
	b.life = proj->life;
	b.frictx = b.fricty = proj->frict;
	b.fam = data->isPlayer() ? Family::Player : Family::Nmy;
	game->bulMan->addBullet(b);
}

void Entity::onDeath(){
	blinking = 0.5f;	

	if (!data->isPlayer()) {
		vec2 dist = game->player->getPos() - getPos();
		dist = dist.getNormalizedSafeZero();
		dist *= 2.0f;
		game->screenshake(0.1f, dist.x, dist.y);
	}
	game->freezeFrame(0.05f);

	if( data->name=="boss"){
		game->explode(x, y,0);
		rs::Timer::delay(500,[]() {
			Game::me->endscreen();
		});
	}
	if (data->name == "player") {
		game->explode(x, y, 0);
		rs::Timer::delay(500, []() {
			Game::me->endscreen();
		});
	}
}

bool Entity::isDead(){
	return hp <= 0;
}

void Entity::updateHits(){
	int result = -1;
	Family fam = Family::Nmy;
	if (data->isNmy())
		fam = Family::Player;

	game->bulMan->testBullet(x, y, fam, 16>>1, result);
	
	if (result >= 0) {
		//recoil
		int bulx = game->bulMan->x[result];
		int buly = game->bulMan->y[result];
		int buldx = game->bulMan->dx[result];
		int buldy = game->bulMan->dy[result];
		dx += buldx * 0.01f;
		dy += buldy * 0.01f;

		//
		int dmg = game->bulMan->getBulletDmg(result);

		bloodSplash(bulx,buly);

		auto proj = game->bulMan->proj[result];
		if (proj->name == "rocket")
			game->explode(bulx, buly, proj);

		hit(dmg, 0);
		 
		game->bulMan->destroy(result);
	}
}

void Entity::bloodSplash(int px, int py){
	rd::Rand& rand = Rand::get();
	for (int i = 0; i < 16; ++i) {

		int sz = rand.dice(1, 2);
		r2::Graphics* sp = r2::Graphics::rect(-sz * 0.5f, -sz * 0.5f, sz, sz, 0xff0000, 1.0f, parent);
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
		p->groundY = y + rand.dice(12, 24);
		p->useGround = true;
	}
}
bool Entity::isActivated()
{
	if (data->isPlayer())
		return true;

	return ( (getPixelPos() - game->player->getPixelPos()).getNorm()) < 500;
}

void Entity::im(){
	using namespace ImGui;
	if (TreeNode("data")) {
		Value("name", data->name);
		DragFloat("speed", &data->speed,0.001f,0,10);
		Value("hp", data->hp);
		Value("tags", data->tags);
		TreePop();
	}

	Value("hp", hp);

	Value("x", x);
	Value("y", y);

	Value("rx", rx);
	Value("cx", cx);

	Value("ry", ry);
	Value("cy", cy);

	SliderFloat("frictX", &frictX, 0.8f, 1.0f);
	SliderFloat("frictY", &frictY, 0.8f, 1.0f);

	Value("cooldown", cooldown);
	DragFloat("progress", &progress, 0, 10);

	r2::Im::imNodeListEntry("this",this);
	r2::Im::imNodeListEntry("spr",spr);
	Super::im();
}

bool Entity::isStunned(){
	return stunDur >= 0.0f;
}

void Entity::update(double dt) {
	Super::update(dt);

	stunDur -= dt;

	if (!isDead() && isActivated() && !isStunned()) {
		if( data->isNmy()){
			if (data->name == "imp") {
				vec2 toPlayer = vec2(game->player->x -x,game->player->y -y);
				toPlayer = toPlayer.getNormalizedSafeZero();
				float scale = 0.55f;
				float speed = 0.66;

				dx *= 0.8f;
				dx = toPlayer.x * speed;
				dy = toPlayer.y * speed;
			}
			if (data->name == "blob") {
				vec2 toPlayer = vec2(game->player->x - x, game->player->y - y);
				toPlayer = toPlayer.getNormalizedSafeZero();
				float scale = 0.55f;
				float speed = 0.4f;
				if (rd::Rand::get().pc(5))
					speed *= 64.0f;
				dx *= 0.98f;
				dy *= 0.98f;
				dx = toPlayer.x * speed;
				dy = toPlayer.y * speed;
			}
		}
	}

	if (!isDead() && isActivated()) {
		updateMovement(dt);

		bool run = false;
		run |= abs(dx) > 0.01f || abs(dy) > 0.01f;
		if (!isActivated())
			run = false;

		if( run ){
			spr->player.speed = 1.0f;
		}
		else {
			spr->player.speed = 0.2f;
		}
		updateHits();
	}

	auto& rnd = rd::Rand::get();
		
	if (!isDead() && isActivated() && data->isNmy()) {
		if( rnd.pc(0.4f)){
			auto player = game->player;
			if (data->name == "imp") {
				auto proj = Data::projs["imp_bullet"];
				fire(player->getPos().x, player->getPos().y, proj);
			}
			if (data->name == "blob") {
				auto proj = Data::projs["blob_bullet"];
				fire(player->getPos().x, player->getPos().y, proj);
			}
			if (data->name == "boss") {
				auto proj = Data::projs["boss_bullet"];
				fire(player->getPos().x, player->getPos().y, proj);
			}
		}
	}
	al.update(dt);

	syncPos();

	if (fadingOut) {
		spr->alpha -= 0.0f;
		if (spr->alpha < 0) {
			detach();
		}
	}

	if(blinking>0){
		int fr = (rs::Timer::frameCount+uid) % 8;
		spr->alpha = (fr < 4);
		blinking -= dt;
		if (blinking <= 0)
			spr->alpha = 1;
	}
}

void Entity::setGridPos(int x, int y){
	cx = x;
	cy = y;

	rx = 0.5;
	ry = 0.5;

	x = std::lrint((cx + rx) + Cst::GRID);
	y = std::lrint((cy + ry) + Cst::GRID);
	trsDirty = true;
}

void Entity::setPixelPos(float x, float y) {
	setPixelPos({ x,y });
}
void Entity::setPixelPos(const Vector2& pos) {
	x = pos.x;
	y = pos.y;

	cx = pos.x / Cst::GRID;
	cy = pos.y / Cst::GRID;

	rx = 1.0f * (x - cx * Cst::GRID) / Cst::GRID;
	ry = 1.0f * (y - cy * Cst::GRID) / Cst::GRID;
	trsDirty = true;
}

Vector2 Entity::getPixelPos(){
	return Vector2((cx + rx) * Cst::GRID, (cy + ry) * Cst::GRID);
}

void Entity::updateMovement(double dt){

	rx += dx * dt;
	ry += dy * dt;

	dx = frictX * dx;
	dy = frictX * dy;

	while( rx > 1 ){
		if( game->isWallGrid(cx+rx+1,cy+ry) ){
			rx = 0.99f;
			break;
		}
		if (game->isWallGrid(cx + rx + 1, cy + ry -1)) {
			rx = 0.99f;
			break;
		}
		rx--;
		cx++;
	}

	while (rx < 0) {
		if (game->isWallGrid(cx + rx, cy + ry)) {
			rx = 0.01f;
			break;
		}
		if (game->isWallGrid(cx + rx, cy + ry-1)) {
			rx = 0.01f;
			break;
		}
		rx++;
		cx--;
	}

	bool isTall = true;

	while (ry > 1) {
		int testY = cy + ry;
		if (isTall) testY+=1.0f;
		if (game->isWallGrid(cx + rx, testY)) {
			ry = 0.99f;
			break;
		}
		ry--;
		cy++;
	}

	while (ry < 0) {
		int testY = cy + ry;
		//if (isTall) testY--;
		if (game->isWallGrid(cx + rx, testY)) {
			ry = 0.01f;
			break;
		}
		ry++;
		cy--;
	}
}

void Entity::syncPos(){
	x = std::lrint((cx + rx) * Cst::GRID);
	y = std::lrint((cy + ry) * Cst::GRID);
	trsDirty = true;
}

void Entity::hit(int dmg, EntityData* by) {
	if (stunDur < 0)stunDur = 0;
	stunDur += 0.5f;
	hp -= dmg;
	bool dead = isDead();
	if (dead) {
		onDeath();
		if (data->isNmy()) {
			game->onFrag();
		}
		if (by == nullptr) {
			//rd::Garbage::trash(this);
			detach();
		}
		else {
			fadingOut = true;
		}
	}
	else {
		if (blinking < 0)blinking = 0;
		blinking += 0.2f;
	}
}

void Entity::fire(Entity&opp) {
	vec2 pos = opp.getPixelPos();
	fire(pos.x,pos.y);
}

bool EntityData::isPlayer(){
	return hasTag("player");
}

bool EntityData::isNmy(){
	return hasTag("nmy");
}

bool EntityData::hasTag(const char* tag){
	for (auto& s : tags)
		if (s == tag)
			return true;
	return false;
}
