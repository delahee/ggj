#include "stdafx.h"

#include "Data.hpp"
#include "r2/im/TilePicker.hpp"
#include "Entity.hpp"
#include "Data.hpp"

rd::TileLib* Data::assets = 0;

std::unordered_map<Str, EntityData*> Data::entities;
std::unordered_map<Str, ProjData*> Data::projs;

void Data::update(double dt) {
	
}

void Data::init(){
	assets = r2::im::TilePicker::getOrLoadLib("assets.xml");

	{
		auto d = new EntityData();
		d->name = "imp";
		d->tags.push_back("nmy");
		d->hp -= 3;
		entities[d->name] = d;
	}
	{
		auto d = new EntityData();
		d->name = "blob";
		d->tags.push_back("nmy");
		d->hp += 3;
		entities[d->name] = d;
	}
	{
		auto d = new EntityData();
		d->name = "player";
		d->sprName = "Doom slayer pixel";
		d->tags.push_back("player");
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
		auto d = new ProjData();
		d->name = "imp";
		d->tags.push_back("nmy");
		d->dmg = 3;
		projs[d->name] = d;
	}
}

