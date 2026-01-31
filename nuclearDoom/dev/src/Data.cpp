#include "stdafx.h"

#include "Data.hpp"
#include "r2/im/TilePicker.hpp"
#include "Entity.hpp"

rd::TileLib* Data::assets = 0;
std::unordered_map<Str, EntityData*> Data::entities;

void Data::update(double dt) {
	
}

void Data::init(){
	assets = r2::im::TilePicker::getOrLoadLib("assets.xml");

	{
		auto d = new EntityData();
		d->name = "imp";
		d->tags.push_back("nmy");
		entities[d->name] = d;
	}
	{
		auto d = new EntityData();
		d->name = "player";
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

}