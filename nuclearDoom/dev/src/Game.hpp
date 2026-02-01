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
class Player;
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
	bool			enabled = true;

	vec2			getGameMousePos();

					virtual ~Game();
	virtual void	dispose() override;
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

	bool			isWallPix(float px, float py);
	bool			isWallGrid(int cx, int cy);
	//void			hitWallPix(float px, float py);
	void			explode(int toX, int toY, ProjData* proj);
	void			hitWallPix(float px, float py, ProjData* proj);
	void			bloodsplash(int px, int py);

	void			freezeFrame(float dur);
	void			screenshake(float dur, float dx, float dy);
	bool			isShaking() { return shakeDur > 0.0f; };

	void			endscreen();
protected:
	float			freezeDur = -1.0f;
	float			shakeDur = -1.0f;

	float			shakeX = 0.0f;
	float			shakeY = 0.0f;

	float			ocamX = 0.0f;
	float			ocamY = 0.0f;
};

