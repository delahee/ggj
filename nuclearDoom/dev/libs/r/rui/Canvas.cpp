#include "stdafx.h"
#include "Canvas.hpp"
#include "r2/tools/Scaler.hpp"

using namespace r2;
using namespace rui;
using namespace r2::tools;
using namespace rd;
//using namespace dtui;

bool Canvas::DEBUG = false;

Canvas::Canvas(r2::Node* parent) : r2::Node(parent) {
	onResize(rs::Display::getSize());
	setName("Canvas");
}

void Canvas::resetContent() {
	resetTRS();
	if (enabled) {
		onResize(rs::Display::getSize());
	}
}

void Canvas::runAndRegResize(std::function<void(void)> f) {
	f();
	sigOnResize.add(f);
}

vec2 Canvas::getCenter(){
	return vec2(getActualSize().x*0.5f,getRefSize().y*0.5f);
}


r::Vector2 Canvas::getMousePos(){
	return r::Vector2(rs::Sys::mouseX / scaleX, rs::Sys::mouseY / scaleY);
}

Vector2 Canvas::getWorkingSize() {
	return vec2(getActualSize().x, getRefSize().y);
}

Vector2 Canvas::getRefSize() {
	Vector2 refSize(-1, -1);
	
	if (refSize.x == -1 && refSize.y == -1) {
		refSize.x = fallbackRefSize.x;
		refSize.y = fallbackRefSize.y;
	}
	else {//desperate are we
		refSize = getSize();
	}
	
	return refSize;
}

float Canvas::getScaleX() { return scaleX; }
float Canvas::getScaleY() { return scaleY; }

Vector2 Canvas::getActualSize() {
	float actualW = rs::Display::width();
	float actualH = rs::Display::height();

	float scX = getScaleX();
	float scY = getScaleY();

	return Vector2(
		actualW / scX,
		actualH / scY
	);
}

void Canvas::onRemoveChild(r2::Node* n) {
}


Canvas::~Canvas() {
	traverse([=](auto n) {//undock everything under my direct control
		undock(n);
	});
	dispose();
}

void Canvas::dispose(){
	for (auto& p : resizeHolder)
		sigOnResize.remove(p.second);
	resizeHolder.clear();
	r2::Node::dispose();
}

float Canvas::getPixelRatio() const {
	return scaleY;
}

bool Canvas::isDocked(r2::Node* spr) {
	return resizeHolder.find(spr) != resizeHolder.cend();
}

/**
* Docking will not take internal pivot into account,
* it will make its best to respect docking propertis for a top-left based sprite based on width and height properties
* which means for a text, you better not use blockAlign ( our model is not very good since 'block align' and 'position align' should be separated )
*/

r2::Node* Canvas::dockLeft(r2::Node* spr, float ofs) { dock(spr, rd::Dir( rd::LEFT | rd::UP_DOWN), ofs); return spr; }

void Canvas::undock(r2::Node* spr) {
	if (!spr)
		return;

	spr->nodeFlags &= ~NF_ENGINE_DOCKED;

	auto pos = resizeHolder.find(spr);
	while(resizeHolder.end() != pos) {
		auto& f = pos->second;
		sigOnResize.remove(f);
		resizeHolder.erase(pos);
		pos = resizeHolder.find(spr);
	}
}

void Canvas::dumpDocked() {
	printf("*** Docked list ***");
	for(auto& s : resizeHolder)
		printf("0x%p %s\n", s.first, s.first->name.c_str());
}

r2::Node* Canvas::dock(r2::Node* spr, rd::Dir dir, float ofs) {
	if( isDocked(spr))
		undock(spr);

	if(DEBUG) printf("docking 0x%p %s\n", spr,spr->name.c_str());
	rui::dock(spr, this, dir, ofs);

	spr->nodeFlags |= NF_ENGINE_DOCKED;
	resizeHolder[spr] = sigOnResize.add([=]() {
		rui::dock(spr, this, dir, ofs);
	});
	spr->onDestruction.addOnce([=]() {
		rd::Bits::unset(spr->nodeFlags, NF_ENGINE_DOCKED);
		if (DEBUG) printf("undocking 0x%p %s \n", spr, spr->name.c_str());
		auto iter = resizeHolder.find(spr);
		while(iter != resizeHolder.end()) {
			sigOnResize.remove(iter->second);
			resizeHolder.erase(iter);
			iter = resizeHolder.find(spr);
		}
	});
	return spr;
}

r2::Node* Canvas::dock(r2::Node* spr, rd::Dir dir, Vector2 ofs) {
	if (isDocked(spr))
		undock(spr);

	if (DEBUG) printf("docking 0x%p %s\n", spr, spr->name.c_str());
	rui::dock(spr, this, dir, ofs);

	spr->nodeFlags |= NF_ENGINE_DOCKED;
	resizeHolder[spr] = sigOnResize.add([=]() {
		rui::dock(spr, this, dir, ofs);
	});
	spr->onDestruction.addOnce([=]() {
		rd::Bits::unset(spr->nodeFlags , NF_ENGINE_DOCKED);
		if (DEBUG) printf("undocking 0x%p %s \n", spr, spr->name.c_str());
		auto iter = resizeHolder.find(spr);
		while(iter != resizeHolder.end()) {
			sigOnResize.remove((*iter).second);
			resizeHolder.erase(iter);
			iter = resizeHolder.find(spr);
		}
	});
	return spr;
}

void rui::Canvas::redock(r2::Node* spr){
	if (!rs::Std::exists(resizeHolder, spr))
		return;
	auto hld = resizeHolder[spr];
	if(hld) 
		hld->function();
}

/**
* Docking will not take internal pivot into account,
* it will make its best to respect docking propertis for a top-left based sprite based on width and height properties
* which means for a text, you better not use blockAlign ( our model is not very good since 'block align' and 'position align' should be separated )
*/

r2::Node* Canvas::dockOnce(r2::Node* spr, rd::Dir dir, Vector2 ofs) {
	//not really useful
	//if(DEBUG) printf("docking once 0x%lx \n", spr);
	rui::dock(spr, this, dir, ofs);
	return spr;
}

r2::Node* Canvas::dockUp(r2::Node* spr, float ofs) {
	dock(spr, rd::Dir(rd::Dir::UP | rd::Dir::LEFT_RIGHT), ofs); return spr;
}

void Canvas::onResize(const Vector2& ns) {
	if (!enabled)
		return;

	Vector2 refSize = getRefSize();
	Scaler::topLeftContain(Vector2(refSize.x, refSize.y), rs::Display::getSize(), this);

	sigOnResize.trigger();
	r2::Node::onResize(ns);
}

void Canvas::im(){
	using namespace ImGui;
	if(TreeNode("Canvas")){
		DragInt2("Fallback Ref size", fallbackRefSize.ptr());
		Value("actual ref size",getRefSize());
		Value("actual size", getActualSize());
		TreePop();
	}
	Super::im();
}

NodeType Canvas::getType() const { 
	return NodeType::NT_RUI_CANVAS;
}

void Canvas::serialize(Pasta::JReflect& f, const char* name){
	if (name) f.visitObjectBegin(name);
	Super::serialize(f, 0);
	f.visit(enabled,"enabled");
	f.visit(fallbackRefSize,"fallbackRefSize");
	if (name) f.visitObjectEnd(name);
}
