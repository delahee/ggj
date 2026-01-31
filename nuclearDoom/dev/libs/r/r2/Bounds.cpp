#include "stdafx.h"

#include "Bounds.hpp"

using namespace rs;
using namespace r2;

Bounds& r2::Bounds::scale(double dx, double dy) {
	*this = fromCenterSize(getCenterX(), getCenterY(), getWidth() * dx, getHeight() * dy);
	return *this;
}

Bounds& r2::Bounds::expand(int px, int py) {
	xMin -= px;
	yMin -= px;
	xMax += px;
	yMax += px;
	return *this;
}

double Bounds::left() const {
	return xMin;
}

double Bounds::right() const {
	return xMax;
}

double Bounds::down() const {
	return yMax;
}

double Bounds::top() const {
	return yMin;
}

double Bounds::up() const {
	return top();
}

double Bounds::bottom() const {
	return down();
}

Bounds& r2::Bounds::load4(double x, double y, double width, double height) {
	xMin = x;
	yMin = y;
	xMax = x + width;
	yMax = y + height;
	return *this;
}

Bounds r2::Bounds::getTransformed(const Pasta::Matrix44& mat) {
	Bounds b = *this;
	b.transform(mat);
	return b;
}

void r2::Bounds::transform(const Pasta::Matrix44& mat) {
	if (isEmpty())//ok let's pray for someone's soul here
		return;

	double l = left();
	double t = top();
	double b = bottom();
	double r = right();

	empty();

	Pasta::Vector3 topLeft = mat * Pasta::Vector3(l, t, 0.f);
	addPoint(topLeft);
	Pasta::Vector3 topRight = mat * Pasta::Vector3(r, t, 0.f);
	addPoint(topRight);
	Pasta::Vector3 bottomLeft = mat * Pasta::Vector3(l, b, 0.f);
	addPoint(bottomLeft);
	Pasta::Vector3 bottomRight = mat * Pasta::Vector3(r, b, 0.f);
	addPoint(bottomRight);
}

std::string Bounds::toString() {
	std::string str;
	str += "tlbr(";
	str += std::to_string(left());
	str += ',';
	str += std::to_string(top());
	str += ',';
	str += std::to_string(right());
	str += ",";
	str += std::to_string(bottom());
	str += ')';
	return str;
}

double r2::Bounds::width() const {
	if (isEmpty())
		return 0.0;
	return abs(right() - left());
}

double r2::Bounds::height() const {
	if (isEmpty())
		return 0.0;
	return abs(bottom() - top());
}

int BoundsI::left() const {
	return xMin;
}

int BoundsI::right() const {
	return xMax;
}

int BoundsI::down() const {
	return yMax;
}

int BoundsI::top() const {
	return yMin;
}

int BoundsI::up() const {
	return top();
}

int BoundsI::bottom() const {
	return down();
}

bool r2::BoundsI::testCircle(int px, int py, int r) {
	int closestX = std::clamp(px, xMin, xMax);
	int closestY = std::clamp(py, yMin, yMax);

	int distX = px - closestX;
	int distY = py - closestY;

	double distSq = distX * distX + distY * distY;
	return distSq < r* r;
}

BoundsI r2::BoundsI::fromCenterSize(int x, int y, int w, int h) {
	BoundsI b;
	b.xMin = std::lrint(x - w * 0.5);
	b.yMin = std::lrint(y - w * 0.5);
	b.xMax = std::lrint(x + w * 0.5);
	b.yMax = std::lrint(y + h * 0.5);
	return b;
}

std::string BoundsI::toString() {
	std::string str;
	str += "tl br(";
	str += std::to_string(left());
	str += ",";
	str += std::to_string(top());
	str += ") , (";
	str += std::to_string(right());
	str += ",";
	str += std::to_string(bottom());
	str += ") ";

	str += std::to_string(width());
	str += "x";
	str += std::to_string(height());
	return str;
};

void BoundsI::im() {
	using namespace ImGui;
	DragInt("min", &xMin);
	DragInt("max", &xMax);
}

void r2::Bounds::im() {
	using namespace ImGui;
	DragDouble2("min", &xMin);
	DragDouble2("max", &xMax);
}