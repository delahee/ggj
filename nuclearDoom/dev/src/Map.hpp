#pragma once
#include "LDTK/Data.hpp"
class Map : public r2::Node {
public:

	ldtk::IntGrid		groundA;
	ldtk::IntGrid		groundB;
	ldtk::IntGrid		hardWalls;
	ldtk::IntGrid		softWalls;
	vec2i				playerSpawn;

						Map(r2::Node* parent);

	rd::StaticBatch*	groundBatch{};
	rd::StaticBatch*	wallBatch{};
	rd::StaticBatch*	topBatch{};
	Pasta::TextureData* data{};

	vec2				getCenter();
	void				im();
};