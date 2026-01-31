#include "stdafx.h"
#include "Scaler.hpp"

using namespace r2;
using namespace r2::tools;
void Scaler::reset(r2::Node* src)
{
	src->scaleY = src->scaleX = 1.0;
	src->x = 0;
	src->y = 0;
}

void Scaler::scale(double factor, r2::Node* src)
{
	src->scaleY = src->scaleX = factor;
	src->x = 0;
	src->y = 0;
}

//ex 640x360 => 1440 x 900
void Scaler::topLeftContain(r::Vector2 srcSize, r::Vector2 destSize, r2::Node* src) {
	float ratioSrcDst = destSize.y / srcSize.y;
	src->x = 0;
	src->y = 0;
	src->scaleY = ratioSrcDst;

	src->scaleX = src->scaleY;
}

void Scaler::centeredBox(r::Vector2 srcSize, r::Vector2 destSize, r2::Node* src)
{
	float srcRatio = srcSize.x / srcSize.y;
	float dstRatio = destSize.x / destSize.y;

	if (dstRatio <= srcRatio) {
		double factor = destSize.x / srcSize.x;

		src->scaleY = src->scaleX = factor;

		src->x = destSize.x * 0.5 - srcSize.x * 0.5 * factor;
		src->y = destSize.y * 0.5 - srcSize.y * 0.5 * factor;
	}
	else {
		double factor = destSize.y / srcSize.y;

		src->scaleY = src->scaleX = factor;

		src->x = destSize.x * 0.5 - srcSize.x * 0.5 * factor;
		src->y = destSize.y * 0.5 - srcSize.y * 0.5 * factor;
	}
}

void Scaler::centeredContain(
	r::Vector2		srcSize,
	r::Vector2		destSize,
	r2::Node* src)
{
	float srcRatio = srcSize.x / srcSize.y;
	float dstRatio = destSize.x / destSize.y;

	if (dstRatio <= srcRatio) {
		double factor = destSize.y / srcSize.y;

		src->scaleY = src->scaleX = factor;

		src->x = destSize.x * 0.5 - srcSize.x * 0.5 * factor;
		src->y = destSize.y * 0.5 - srcSize.y * 0.5 * factor;
	}
	else {
		double factor = destSize.x / srcSize.x;

		src->scaleY = src->scaleX = factor;

		src->x = destSize.x * 0.5 - srcSize.x * 0.5 * factor;
		src->y = destSize.y * 0.5 - srcSize.y * 0.5 * factor;
	}
};

void Scaler::fill(
	r::Vector2		srcSize,
	r::Vector2		destSize,
	r2::Node* src)
{
	src->x = 0;
	src->y = 0;

	src->scaleX = destSize.x / srcSize.x;
	src->scaleY = destSize.y / srcSize.y;
}