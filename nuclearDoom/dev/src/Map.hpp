#pragma once
#include "LDTK/Data.hpp"
class Map : public r2::Node {
public:

	ldtk::IntGrid		groundA;
	ldtk::IntGrid		groundB;
	ldtk::IntGrid		hardWalls;
	ldtk::IntGrid		softWalls;
	vec2i				playerSpawn;

	std::vector<vec2i>	impList;
	std::vector<vec2i>	blobList;
	std::vector<vec2i>	bossList;

						Map(r2::Node* parent);

	rd::StaticBatch*	groundBatch{};
	rd::StaticBatch*	wallBatch{};
	rd::StaticBatch*	topBatch{};
	Pasta::TextureData* data{};

	rd::Rand			rnd;
	vec2				getCenter();
	void				im();
	void				genGround(int x, int y);
};