#pragma once

class Game;

enum class Family{
	Unknown,
	Player,
	Nmy,
};

struct Bullet{
	float	x = 0;
	float	y = 0;
	float	dx = 0;
	float	dy = 0;
	float	frictx = 0.99f;
	float	fricty = 0.99f;
	int		life = 120;
	int		dmg = 1;
	Str		sprName;
	Family  fam = Family::Unknown;
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
	std::vector<int>				dmg;
	std::vector<Family>				fam;
	std::vector<Behavior>			bhv;
	std::vector<rd::ABatchElem*>	spr;

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