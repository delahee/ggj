#pragma once

class Game;

enum class Family{
	Unknown,
	Player,
	Nmy,
};

struct Bullet{
	float		x = 0;
	float		y = 0;
	float		dx = 0;
	float		dy = 0;
	float		frictx = 0.99f;
	float		fricty = 0.99f;
	float		life = 120.0f;
	int			dmg = 1;
	int			flags = 0;
	Str			sprName;
	Family		fam = Family::Unknown;
	ProjData*	proj = 0;
};

class BulMan : public rd::Agent {
public:
	enum Behavior {
		Straight,
	};

	enum Flags : s16{
		Alive = 1<<0,
		RandRotation=1<<1,
		AlignedRotation=1<<2,
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
	std::vector<int>				dmg;
	std::vector<Family>				fam;
	std::vector<rd::ABatchElem*>	spr;
	std::vector<ProjData*>			proj;

	Game *							game = 0;		
	r2::Batch*						rdr = 0;
									BulMan(Game * g, rd::AgentList* al);
	virtual void					update(double dt);

	void							addBullet( Bullet b );
	int								getBulletDmg(int idx);
	//result < 0 if negative
	void							testBullet(float px, float py, Family fam, int bSize,int & result);

	void							destroy(int idx);
	void							im(int idx);

protected:
	void							swap(int idxA, int idxB);
	void							onInactive(int idxA);

};