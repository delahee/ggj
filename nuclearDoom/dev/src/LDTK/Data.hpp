#pragma once

#include <string>
#include "r/Color.hpp"
#include "rd/Dir.hpp"
#include "rd/Vars.hpp"

namespace ldtk {
	struct NeighbourLevels {
		std::string levelIid;
		rd::Dir		dir = rd::Dir::DOWN;
	};

	struct Entity{
		std::string id;
		std::string iid;
		std::string layer;
		int x{};
		int y{};
		int width{};
		int height{};
		r::Color color;

		rd::Vars customFields;
	};

	struct EntityLayer{
		std::string				name;
		std::vector<Entity>		instances;
	};

	class Data {
	public:
		std::string identifier;
		std::string uniqueIdentifer;
		int x{};
		int y{};
		int width{};
		int height{};
		r::Color bgColor{};
		// customFields
		std::vector<std::string> layers;
		std::vector<EntityLayer> entities;

		void serialize();
	};
}