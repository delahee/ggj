#pragma once

#include "Entity.hpp"

class Game;

enum Gun {
	Shotgun,
	Plasma,
	Rocket,
};

class Player : public Entity {
public:
	Gun				gun = Shotgun;

					Player(Game*g,r2::Node* parent);

	virtual void	update(double dt)override;
	void			fire();

	void			controlsMove(vec2 move,double dt);
	void			controls(double dt);

};