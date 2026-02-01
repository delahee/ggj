#pragma once
#include <unordered_map>

struct EntityData;
struct ProjData;

class Data {
public:
	static rd::TileLib* assets;

	static std::unordered_map<Str, EntityData*>  entities;
	static std::unordered_map<Str, ProjData*>	projs;


	static void			init();
	static void			update(double dt);
	static ProjData*	getProj(const char* name);

};