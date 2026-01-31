#include "stdafx.h"

#include <unordered_map>
#include <EASTL/variant.h>

#include "r2/Node.hpp"
#include "rd/ABitmap.hpp"
#include "rui/Canvas.hpp"
#include "r2/StaticBox.hpp"

#include "BulMan.hpp"
#include "Game.hpp"
#include "rd/JSerialize.hpp"
#include "rd/Garbage.hpp"
#include "Entity.hpp"
#include "r2/fx/Part.hpp"
#include "FX.hpp"
#include "Map.hpp"
#include "UI.hpp"

using namespace rd;
using namespace std::string_literals;

void Game::onFrag(){
	frags++;

}

static bool defeated = false;
void Game::defeat(){
	if (defeated)
		return;

}

void Game::intro(){
	

}

void Game::sfx(const char * name){
	rd::AudioMan::get().playFile(name);
}

void Game::beginGame(){
	
}

void Game::victory() {

	
}

void Game::hit() {
	
}

void Game::controls(double dt){
	player->controls(dt);
}

Game::Game(r2::Node* _root, r2::Scene* sc, rd::AgentList* parent) : Super(parent) {
	scRoot = _root;
	root = new r2::StaticBox(r2::Bounds::fromTLWH(0, 0, Cst::W, Cst::H), scRoot);

	Data::init();

	rd::AudioMan::get().init();
	map = new Map(root);
	ui = new UI(root);
	bulMan = new BulMan(this,&al);
	player = new Player(this,root);
	player->init("player");
	player->setPixelPos(Cst::W/2, Cst::H/2);

	auto e = new Entity(this,root);
	e->init("imp");
	e->setPixelPos(100, 100);
	nmies.push_back(e);
	
}

void Game::update(double dt) {
	Super::update(dt);

	controls(dt);
	al.update(dt);
	tw.update(dt);
#ifdef PASTA_DEBUG
	im();	
#endif
}
/*
template <> void Pasta::JReflect::visit(std::vector<Vector2> & v, const char* name) {
	u32 arrSize = v.size()*2;
	if (visitArrayBegin(name, arrSize)) {
		if (isReadMode())
			if (v.size() < arrSize >> 1)
				v.resize(arrSize >> 1);
		for (u32 i = 0; i < arrSize; ++i) {
			visitIndexBegin(i);
			Vector2& vf = v[i>>1];
			if( 0==(i & 1) )
				visit(vf.x, nullptr);
			else
				visit(vf.y, nullptr);
			visitIndexEnd();
		}
	}
	visitArrayEnd(name);
}

static void visitEastl(Pasta::JReflect&jr,eastl::vector<Vector2>& v, const char* name) {
	u32 arrSize = v.size() * 2;
	if (jr.visitArrayBegin(name, arrSize)) {
		if (jr.isReadMode())
			if( v.size() < arrSize >>1)
				v.resize(arrSize >> 1);
		for (u32 i = 0; i < arrSize; ++i) {
			jr.visitIndexBegin(i);
			Vector2& vf = v[i >> 1];
			if (0 == (i & 1))
				jr.visit(vf.x, nullptr);
			else
				jr.visit(vf.y, nullptr);
			jr.visitIndexEnd();
		}
	}
	jr.visitArrayEnd(name);
}
*/


bool Game::im(){
	using namespace ImGui;
	static bool opened = true;
	if (Begin("Game", &opened)) {
		if (TreeNode("player")) {
			player->im();
			TreePop();
		}
		int idx = 0;
		for (auto e : nmies){
			if (TreeNode("Nmy"s + std::to_string(idx))) {
				e->im();
				TreePop();
			}
			idx++;
		}
		idx = 0;
		
		for (; idx < bulMan->nbActive;++idx) {
			if (TreeNode("Bul "s + std::to_string(idx))) {
				bulMan->im(idx);
				TreePop();
			}
			idx++;
		}
		End();
	}
	return false;
}
