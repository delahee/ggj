	#include "stdafx.h"

#include "Game.hpp"
#include "Entity.hpp"
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

void Entity::im(){
	using namespace ImGui;
	if (TreeNode("data")) {
		Value("name", data->name);
		DragFloat("speed", &data->speed,0.001f,0,10);
		Value("hp", data->hp);
		Value("good", data->good);
		TreePop();
	}

	Value("x", x);
	Value("y", y);

	Value("rx", rx);
	Value("cx", cx);

	Value("ry", ry);
	Value("cy", cy);

	Value("cooldown", cooldown);
	DragFloat("progress", &progress, 0, 10);

	r2::Im::imNodeListEntry("this",this);
	r2::Im::imNodeListEntry("spr",spr);
	Super::im();
}

void Entity::update(double dt) {
	Super::update(dt);

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
			rx = 0.99;
			break;
		}
		rx--;
		cx++;
	}

	while (rx < 0) {
		if (game->isWallGrid(cx + rx, cy + ry)) {
			rx = 0.01;
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
			ry = 0.99;
			break;
		}
		ry--;
		cy++;
	}

	while (ry < 0) {
		int testY = cy + ry;
		if (isTall) testY--;
		if (game->isWallGrid(cx + rx, testY)) {
			ry = 0.01;
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

	bool dead = hp <= 0;
	if (dead) {
		if (data->isMonster()) {
			game->onFrag();
		}
		if (by == nullptr) {
			rd::Garbage::trash(this);
		}
		else {
			if (by->name == "bike_park") {
				init(Data::entities["bike"]);
				fadingOut = true;
			}
		}
	}
	else {
		blinking = 0.2f;
	}
}

void Entity::fire(Entity*opp) {
	
}

bool EntityData::isMonster() {
	for(auto &s :tags)
		if (s == "monster")
			return true;
	return false;
}
