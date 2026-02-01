#pragma once

class Game;

struct Bullet{
	float	x = 0;
	float	y = 0;
	float	dx = 0;
	float	dy = 0;
	float	frictx = 0.99f;
	float	fricty = 0.99f;
	int		life = 120;
	Str		sprName;
};

class BulMan : public rd::Agent {
public:
	enum Behavior {
		Straight,
	};

	enum Flags : s16{
		Alive
	};

	int								nbActive = 0;
	std::vector<r::s16>				flags;
	std::vector<float>				x;
	std::vector<float>				y;
	std::vector<float>				dx;
	std::vector<float>				dy;
	std::vector<float>				frictX;
	std::vector<float>				frictY;
	std::vector<float>				life;
	std::vector<Behavior>			bhv;
	std::vector<rd::ABatchElem*>	spr;

	Game *							game = 0;		
	r2::Batch*						rdr = 0;
									BulMan(Game * g, rd::AgentList* al);
	virtual void					update(double dt);

	void							addBullet( Bullet b );
	void							swap( int idxA, int idxB );
	void							onInactive( int idxA );

	void							im(int idx);
};