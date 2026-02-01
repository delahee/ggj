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

void Entity::fire(int pixX, int pixY) {
	Bullet b;
	auto ppos = getPixelPos();
	b.x = ppos.x;
	b.y = ppos.y;
	b.sprName = "bullet";

	float speed = 150.0f;

	vec2 dir = { pixX - b.x, pixY - b.y };
	dir = dir.getNormalizedSafeZero();
	b.dx = dir.x * speed;
	b.dy = dir.y * speed;

	if (!b.dx && !b.dy) {
		b.dy = 1;
	}
	b.life = 10.0f;
	b.frictx = b.fricty = 1;
	b.fam = data->isPlayer() ? Family::Player : Family::Nmy;
	game->bulMan->addBullet(b);
}

void Entity::onDeath(){
	blinking = 0.5f;
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
		int bulx = game->bulMan->x[result];
		int buly = game->bulMan->y[result];
		int buldx = game->bulMan->dx[result];
		int buldy = game->bulMan->dy[result];

		dx += buldx * 0.01f;
		dy += buldy * 0.01f;

		int dmg = game->bulMan->getBulletDmg(result);
		hit(result, 0);
		game->bulMan->destroy(result);
	}
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

void Entity::update(double dt) {
	Super::update(dt);

	if (!isDead()) {
		updateMovement(dt);
		updateHits();
	}
	al.update(dt);

	syncPos();

	if (fadingOut) {
		spr->alpha -= 0.01f;
		if (spr->alpha < 0)
			rd::Garbage::trash(this);
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
		if( game->isWallGrid(cx+rx,cy+ry) ){
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
		rx++;
		cx--;
	}

	bool isTall = true;

	while (ry > 1) {
		int testY = cy + ry;
		if (isTall) testY++;
		if (game->isWallGrid(cx + rx, testY)) {
			ry = 0.99f;
			break;
		}
		ry--;
		cy++;
	}

	while (ry < 0) {
		int testY = cy + ry;
		if (isTall) testY--;
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
	hp -= dmg;
	bool dead = isDead();
	if (dead) {
		onDeath();
		if (data->isNmy()) {
			game->onFrag();
		}
		if (by == nullptr) {
			rd::Garbage::trash(this);
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
