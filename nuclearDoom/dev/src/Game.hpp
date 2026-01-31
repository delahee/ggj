#pragma once

#include "rd/Agent.hpp"
#include "rd/ext/Interp.hpp"
#include "UserTypes.hpp"
#include "rgp/Cine.hpp"
#include "Player.hpp"

class Game;
class Map;
class UI;
class BulMan;
class rgp::CineController;

namespace r2 { class Node; }
namespace rui { class Canvas; }

class Game : rd::Agent {
public:
	typedef rd::Agent Super;
	r2::Node*		scRoot = 0;
	r2::Node*		root = 0;
	rd::AgentList	al;

	Map*			map{};
	UI*				ui{};
	BulMan*			bulMan{};

	Player*			player{};
	std::vector<Entity*> nmies;
	std::vector<Entity*> breakable;

	Path*			path=0;
	int				frags = 0;
	rd::Tweener		tw;

	void			intro();
	void			sfx(const char* name);
	void			beginGame();
	void			victory();
	void			defeat();
	void			hit();
	void			controls( double dt);

					Game(r2::Node * root,r2::Scene* sc, rd::AgentList* parent);
	virtual void	update(double dt) override;

	bool			im();

	void			onFrag();
};

