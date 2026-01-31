#pragma once

#include "r2/Node.hpp"

struct EntityData;
struct Path;
class Game;

struct EntityData{
	std::string					name;
	std::string					attack;
	std::vector<Str>			tags;

	bool						good = false;
	float						speed = 10.0f;
	int							hp = 10;
	int							maxUpgrade = 0;
	int							dmg = 1;
	int							range = 30;
	float						cooldown = 1.0f;
	float						projSpeed = 0.1f;

	bool						isMonster();
	bool						isTurret() { return good; };

	static EntityData*			get(const char* name);
};

class Entity : public r2::Node {
	typedef r2::Node Super;

public:
	rd::Dir			dir = rd::Dir::DOWN;
	float			rx = 0;
	float			ry = 0;

	int				cx = 0;
	int				cy = 0;

	float			dx = 0;
	float			dy = 0;

	float			frictX = 0.95f;
	float			frictY = 0.95f;

	int				upgrade = 0;
	float			cooldown = 0.0f;
	Vector2			prevPos;

	float			progress = 0;

	EntityData*		data = 0;
	Path*			path = 0;
	rd::ABitmap*	spr = 0;
	Game*			game = 0;
	rd::AgentList	al;

	bool			fadingOut = false;
	float			blinking = 0.0f;
	bool			invincible = false;

	int				hp = 0;

	enum class State : int {
		Running,
	};

									Entity(Game*g,r2::Node* parent);
					virtual			~Entity();

	void			init(const char * name);
	void			init(EntityData* data);

	void			im();
	virtual void	update(double dt) override;

	void			setPixelPos(float x, float y);
	void			setPixelPos(const Vector2& pos);
	Vector2			getPixelPos();

	void			updateMovement(double dt);

	void			syncPos();

	void			hit(int dmg, EntityData * by = nullptr);

	void			fire(Entity*opp);

	static std::vector<Entity*>	ALL;
};