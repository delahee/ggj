#include "stdafx.h"

#include <unordered_map>
#include <EASTL/variant.h>


#include "r2/Node.hpp"
#include "rd/ABitmap.hpp"
#include "rui/Canvas.hpp"
#include "r2/StaticBox.hpp"

#include "Game.hpp"
#include "rd/JSerialize.hpp"
#include "rd/Garbage.hpp"
#include "Entity.hpp"
#include "r2/fx/Part.hpp"
#include "FX.hpp"
#include "Map.hpp"
#include "UI.hpp"

using namespace rd;
static int towerEverBuild = 0;
static r::Color KIWI = r::Color(0x663931);

void Game::onFrag(){
	frags++;

	if (frags % 3 == 0) {
		auto s = rd::ABitmap::mk("kiwifruit", Data::assets, fragFlow);
		s->setCenterRatio(0.5, 1);
		FX::blink(s);
		sfx("snd/kiwi_up.wav");
	}
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
	sfx("snd/hitplayer.wav");
	if( 0 == livesFlow->nbChildren())
		defeat();
	else {
		livesFlow->children[0]->destroy();
	}
}



Game::Game(r2::Node* _root, r2::Scene* sc, rd::AgentList* parent) : Super(parent) {
	scRoot = _root;
	root = new r2::StaticBox(r2::Bounds::fromTLWH(0, 0, Cst::W, Cst::H), scRoot);

	Data::init();

	rd::AudioMan::get().init();

	map = new Map(root);

	
}

void Game::update(double dt) {
	Super::update(dt);
	al.update(dt);
	tw.update(dt);
#ifdef PASTA_DEBUG
	im();	
#endif
}

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


bool Game::im(){
	return false;
}
