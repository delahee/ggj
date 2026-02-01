#pragma once
#include "Entity.hpp"
class Game;

class Player : public Entity {
public:
					Player(Game*g,r2::Node* parent);

	virtual void	update(double dt)override;

	void			controlsMove(vec2 move,double dt);
	void			controls(double dt);
};