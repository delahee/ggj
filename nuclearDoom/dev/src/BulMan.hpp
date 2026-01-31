#pragma once

class BulMan : public rd::Agent {
public:
	enum Behavior {
		Straight,
	};

	enum Flags : s16{
		Alive
	};

	std::vector<r::s16>		flags;
	std::vector<float>		x;
	std::vector<float>		y;
	std::vector<float>		dx;
	std::vector<float>		dy;
	std::vector<float>		life;
	std::vector<Behavior>	bhv;

	virtual void update(double dt);
};