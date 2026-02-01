#include "stdafx.h"

#include "Game.hpp"
#include "BulMan.hpp"

BulMan::BulMan(Game *g, rd::AgentList* al) : rd::Agent(al){
	game = g;
	int future = 64;
	flags.reserve(future);
	x.reserve(future);
	y.reserve(future);
	dx.reserve(future);
	dy.reserve(future);
	frictX.reserve(future);
	frictY.reserve(future);
	life.reserve(future);
	proj.reserve(future);
	spr.reserve(future);

	rdr = new r2::Batch(g->root);
	rdr->setUniqueName("Bullet Renderer");
}

void BulMan::update(double _dt) {
	float dt = (float) _dt;
	int sz = nbActive;

	for (int idx = 0; idx < sz; ++idx) {
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

		if (vlife < 0.33f){
			auto& vspr = spr[idx];
			vspr->alpha -= dt * 4.0f;
		}

		if (vlife < 0.0f && nbActive){
			swap(idx, nbActive - 1);
			onInactive(nbActive - 1);
			nbActive--;
		}
	}

	sz = nbActive;
	for (int idx = sz - 1; idx >= 0; --idx) {
		auto& vx = x[idx];
		auto& vy = y[idx];
		auto& vproj = proj[idx];

		if( game->isWallPix(vx,vy)){
			game->hitWallPix(vx,vy, vproj);

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
	int res = nbActive + 1;
	flags.resize(res);
	x.resize(res);
	y.resize(res);
	dx.resize(res);
	dy.resize(res);
	frictX.resize(res);
	frictY.resize(res);
	life.resize(res);
	spr.resize(res);
	fam.resize(res);
	dmg.resize(res);
	proj.resize(res);

	int idx = nbActive;
	x[idx] = b.x;
	y[idx] = b.y;
	dx[idx] = b.dx;
	dy[idx] = b.dy;
	life[idx] = b.life;
	frictX[idx] = b.frictx;
	frictY[idx] = b.fricty;
	fam[idx] = b.fam;
	dmg[idx] = b.dmg;
	proj[idx] = b.proj;
	
	auto ab = spr[idx] = rd::ABatchElem::fromPool(b.sprName.c_str(), Data::assets, rdr);
	if (!ab) return;
	
	ab->setCenterRatio();
	ab->setPriority(-b.y);
	ab->setPos(b.x, b.y);
	if (b.flags & RandRotation)
		ab->rotation = rd::Rand::get().angle();
	if (b.flags & AlignedRotation) {
		ab->rotation = atan2(b.dy, b.dx);
	}
	nbActive++;
}

int BulMan::getBulletDmg(int idx){
	if (idx < 0)
		return 0;
	return dmg[idx];
}

void BulMan::onInactive(int idxA){
	if (spr[idxA]) {
		spr[idxA]->visible = false;
		spr[idxA]->toPool();
	}
	spr[idxA] = 0;
}

void BulMan::testBullet(float px, float py, Family pfam, int bSize,int& result){
	int sz = nbActive;
	for (int idx = 0; idx < sz; ++idx) {
		auto& vx = x[idx];
		auto& vy = y[idx];
		auto& vfam = fam[idx];
		if (vfam != pfam)
			continue;
		if( vec2(px - vx, py - vy).getNormSquared() < bSize* bSize ){
			result = idx;
			return;
		}
	}
	result = -1;
}

void BulMan::destroy(int idx)
{
	if (nbActive == 0)
		return;
	swap(idx, nbActive - 1);
	onInactive(nbActive - 1);
	nbActive--;
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
	Value("fam", (int)fam[idx]);
	Value("flags", (int)flags[idx]);
	Value("proj", (int)proj[idx]);
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
	std::swap(dmg[idxA]		, dmg[idxB]);
	std::swap(fam[idxA]		, fam[idxB]);
	std::swap(flags[idxA]	, flags[idxB]);
	std::swap(proj[idxA]	, proj[idxB]);
}
