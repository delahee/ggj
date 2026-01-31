#include "stdafx.h"

#include "Game.hpp"
#include "BulMan.hpp"

BulMan::BulMan(Game *g, rd::AgentList* al) : rd::Agent(al){

	int future = 64;
	flags.reserve(future);
	x.reserve(future);
	y.reserve(future);
	dx.reserve(future);
	dy.reserve(future);
	frictX.reserve(future);
	frictY.reserve(future);
	life.reserve(future);
	bhv.reserve(future);
	spr.reserve(future);

	rdr = new r2::Batch(g->root);
	rdr->setUniqueName("Bullet Renderer");
}

void BulMan::update(double _dt) {
	float dt = (float) _dt;
	int sz = nbActive;

	for (int idx = 0; idx < sz; ++idx) {
		auto& vflags	= flags[idx];
		auto& vx		= x[idx];
		auto& vy		= y[idx];
		auto& vdx		= dx[idx];
		auto& vdy		= dy[idx];
		
		auto& vfrictx	= frictX[idx];
		auto& vfricty	= frictY[idx];

		vx				+= vdx * dt;
		vy				+= vdy * dt;

		float nbFr = _dt / (1.0 / 60.0);

		vdx				*= powf(vfrictx, nbFr);
		vdy				*= powf(vfricty, nbFr);
	}

	for (int idx = sz-1; idx >= 0; --idx) {
		auto& vlife = life[idx];
		vlife -= dt;
		if (vlife < 0 && nbActive){
			swap(idx, nbActive - 1);
			onInactive(nbActive - 1);
			nbActive--;
		}
	}

	for (int idx = 0; idx < sz; ++idx) {
		auto& vspr = spr[idx];
		if (vspr) {
			auto& vx = x[idx];
			auto& vy = y[idx];
			vspr->setPos(vx, vy);
			vspr->setPriority(-vy);
			vspr->update(dt);
		}
		else
			break;
	}
}

void BulMan::addBullet(Bullet b){
	nbActive = x.size();

	int res = nbActive + 1;
	flags.resize(res);
	x.resize(res);
	y.resize(res);
	dx.resize(res);
	dy.resize(res);
	frictX.resize(res);
	frictY.resize(res);
	life.resize(res);
	bhv.resize(res);
	spr.resize(res);

	int idx = nbActive;
	x[idx] = b.x;
	y[idx] = b.y;
	dx[idx] = b.dx;
	dy[idx] = b.dy;
	life[idx] = b.life;
	frictX[idx] = b.frictx;
	frictY[idx] = b.fricty;
	
	auto ab = spr[idx] = rd::ABatchElem::fromPool(b.sprName.c_str(), Data::assets, rdr);
	if (!ab) return;
	
	ab->setCenterRatio();
	ab->setPriority(-b.y);
	ab->setPos(b.x, b.y);
	nbActive++;
}

void BulMan::onInactive(int idxA){
	if (spr[idxA]) {
		spr[idxA]->visible = false;
		spr[idxA]->toPool();
	}
	spr[idxA] = 0;
}

void BulMan::im(int idx) {
	using namespace ImGui;
	Value("x", x[idx]);
	Value("y", y[idx]);
	Value("dx", dx[idx]);
	Value("dy", dy[idx]);
	Value("life", life[idx]);
	Value("frictX", frictX[idx]);
	Value("frictY", frictY[idx]);
	Value("spr", spr[idx]);
}

void BulMan::swap(int idxA, int idxB){
	if (idxA == idxB)
		return;
	std::swap(x[idxA]		, x[idxB]);
	std::swap(y[idxA]		, y[idxB]);
	std::swap(dx[idxA]		, dx[idxB]);
	std::swap(dy[idxA]		, dy[idxB]);
	std::swap(life[idxA]	, life[idxB]);
	std::swap(frictX[idxA]	, frictX[idxB]);
	std::swap(frictY[idxA]	, frictY[idxB]);
	std::swap(spr[idxA]		, spr[idxB]);
}
