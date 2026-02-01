#include "stdafx.h"
#include "Data.hpp"

void ldtk::IntGrid::set(s16 x, s16 y, u32 val)
{
	if (x < 0)
		return;
	if (x >= w)
		return;
	if (y < 0)
		return;
	if (y >= w)
		return;
	bitmap[y * w + x] = val;
}

u32 ldtk::IntGrid::get(s16 x, s16 y)
{
	if (x < 0)
		return 0;
	if (x >= w)
		return 0;
	if (y < 0)
		return 0;
	if (y >= w)
		return 0;
	return bitmap[y*w+x];
}

void ldtk::IntGrid::setSize(s16 w, s16 h){
	this->w = w;
	this->h = h;
	bitmap.resize(w*h, 0);
}
