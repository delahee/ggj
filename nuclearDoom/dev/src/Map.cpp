#include "stdafx.h"
#include "Map.hpp"

Map::Map(r2::Node* parent):r2::Node(parent){
	name = "map";

	r2::Graphics::rect({ -1000,-1000 }, { 2000,2000 }, r::Color(0x7d644e), this);
}
