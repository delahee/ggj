#include "stdafx.h"
#include "Map.hpp"
#include "rd/Rand.hpp"

using namespace std::string_literals;

Map::Map(r2::Node* parent):r2::Node(parent){
	name = "map";

	r2::Graphics::rect({ -1000,-1000 }, { 2000,2000 }, r::Color(0x7d644e), this);

	groundBatch = new rd::StaticBatch(this);
	wallBatch = new rd::StaticBatch(this);
	topBatch = new rd::StaticBatch(this);

	auto bmp = data = rd::RscLib::getTextureData("res/level_a.png");
	int w = bmp->width;
	int h = bmp->height;
	int tw = 0;

	rd::Rand& rand = rd::Rand::get();

	groundA.setSize(w, h);
	groundB.setSize(w, h);
	hardWalls.setSize(w, h);
	softWalls.setSize(w, h);

	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++){
			u8* cal = bmp->getRawTexel(x,y,tw);
			u8 red = *cal;
			u8 green = *(cal+1);
			u8 blue = *(cal+2);
			u8 alpha = *(cal+3);

			u32 col24 = red | green << 8 | blue << 16;

			if( col24 == 0xff00ff)//player spawn
				playerSpawn = { x,y };
			else if(col24== 0xff0000){
				groundA.set(x, y);

				int groundType = rand.dice(1, 1);
				std::string label = "ground"s + std::to_string(groundType);
				auto b = r2::BatchElem::fromLib(Data::assets, label.c_str(), wallBatch);
				b->x = x * Cst::GRID;
				b->y = y * Cst::GRID;
				b->setPriority(-1000);
				b->vars.setTag("ground");

				std::string coo = std::to_string(x) + "_" + std::to_string(y);
				b->vars.setTag(coo.c_str());
			}
			else if(col24== 0xffffff){
				hardWalls.set(x,y);

				int wallType = rand.dice(1, 1);
				std::string label = "hardWall"s + std::to_string(wallType);
				auto b = r2::BatchElem::fromLib(Data::assets, label.c_str(), wallBatch);
				b->x = x * Cst::GRID;
				b->y = y * Cst::GRID;
				b->vars.setTag("wall");
				b->vars.setTag("hard");

				std::string coo = std::to_string(x) + "_" + std::to_string(y);
				b->vars.setTag(coo.c_str());
			}
			else if (col24 == 0x00ff00) {
				softWalls.set(x, y);

				int wallType = rand.dice(1, 7);
				std::string label = "wall"s + std::to_string(wallType);
				auto b = r2::BatchElem::fromLib(Data::assets, label.c_str(), wallBatch);
				b->x = x * Cst::GRID;
				b->y = y * Cst::GRID;
				b->vars.setTag("wall");
				b->vars.setTag("soft");

				std::string coo = std::to_string(x) + "_" + std::to_string(y);
				b->vars.setTag("soft");
				b->vars.setTag(coo.c_str());
			}

		}
	//bmp->getRawTexel();
}

vec2 Map::getCenter(){
	return { data->width *0.5f,data->height *0.5f };
}

void Map::im(){

}