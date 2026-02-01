#include "stdafx.h"

#include "Data.hpp"
#include "r2/im/TilePicker.hpp"
#include "Entity.hpp"
#include "Data.hpp"
#include "BulMan.hpp"

rd::TileLib* Data::assets = 0;

std::unordered_map<Str, EntityData*> Data::entities;
std::unordered_map<Str, ProjData*> Data::projs;

void Data::update(double dt) {
	
}

ProjData* Data::getProj(const char* name){
	if( projs.find(name) == projs.end())
		return projs["dummy"];
	return projs[name];
}

void Data::init(){
	assets = r2::im::TilePicker::getOrLoadLib("assets.xml");
	assets->defaultFrameRate = 10;

	{
		auto d = new EntityData();
		d->name = "imp";
		d->tags.push_back("nmy");
		d->hp += 10;
		entities[d->name] = d;
	}
	{
		auto d = new EntityData();
		d->name = "blob";
		d->tags.push_back("nmy");
		d->hp += 5;
		entities[d->name] = d;
	}
	{
		auto d = new EntityData();
		d->name = "player";
		d->sprName = "Doom slayer pixel";
		d->tags.push_back("player");
#ifdef _DEBUG
		d->hp = 5000;
#endif
		entities[d->name] = d;
	}
	{
		auto d = new EntityData();
		d->name = "dummy";
		d->tags.push_back("dummy");
		entities[d->name] = d;
	}
	{
		auto d = new EntityData();
		d->name = "kiwi";
		d->tags.push_back("kiwi");
		entities[d->name] = d;
	}
	{
		auto d = new EntityData();
		d->name = "boss";
		d->tags.push_back("nmy");
		d->hp += 50;
		entities[d->name] = d;
	}

	{
		auto d = new ProjData();
		d->name = "imp_bullet";
		d->tags.push_back("nmy");
		d->dmg = 3;
		projs[d->name] = d;
	}

	{
		auto d = new ProjData();
		d->name = "blob_bullet";
		d->sprName = "blob_bullet";
		d->tags.push_back("nmy");
		d->flags |= BulMan::AlignedRotation;
		d->dmg = 3;
		projs[d->name] = d;
	}
	{
		auto d = new ProjData();
		d->name = "boss_bullet";
		d->sprName = "boss_bullet";
		d->tags.push_back("nmy");
		d->dmg = 3;
		projs[d->name] = d;
	}

	{
		auto d = new ProjData();
		d->name = "shotgun";
		d->sprName = "bullet_shotgun";
		d->tags.push_back("player");
		d->dmg = 1;
		d->nb = 5;
		d->life = 0.3f;
		d->frict = 0.92f;
		d->speed *= 4.0f;
		d->flags |= BulMan::RandRotation;
		projs[d->name] = d;

	}

	{
		auto d = new ProjData();
		d->name = "plasma";
		d->sprName = "bullet_plasma";
		d->tags.push_back("player");
		d->dmg = 1;
		d->speed *= 2;
		projs[d->name] = d;
	}
	{
		auto d = new ProjData();
		d->name = "rocket";
		d->sprName = "bullet_rocket";

		d->tags.push_back("player");
		d->dmg = 30;
		d->speed *= 0.5;
		projs[d->name] = d;
	}

	{
		auto d = new ProjData();
		d->name = "dummy";
		d->tags.push_back("player");
		d->dmg = 1;
		projs[d->name] = d;
	}
}

