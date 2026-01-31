#include "stdafx.h"

#include "BulMan.hpp"

void BulMan::update(double _dt) {
	float dt = (float) _dt;

	int sz = x.size();

	for (int idx = 0; idx < sz; ++sz) {
		auto& vflags	= x[idx];
		auto& vx		= x[idx];
		auto& vy		= y[idx];
		auto& vdx		= dx[idx];
		auto& vdy		= dy[idx];
		auto& vlife		= life[idx];

		vx				+= vdx * dt;
		vy				+= vdy * dt;
		vlife			-= dt;
	}
}