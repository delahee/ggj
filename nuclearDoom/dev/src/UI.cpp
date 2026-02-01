#include "stdafx.h"
#include "UI.hpp"
#include "Game.hpp"

using namespace std::string_literals;
UI::UI(Game * game,r2::Node * parent) : rui::Canvas(parent){
	this->game = game;
	name = "ui";
}

void UI::update(double dt){
	Super::update(dt);

	auto sc = getScene();
	x = sc->cameraPos.x - 50;
	y = sc->cameraPos.y ;
	trsDirty = true;
	syncAllMatrix();

	if(hp == 0){
		r2::Flow* fl = new r2::Flow(this);
		fl->vertical();
		fl->x = 32;

		hp = r2::Text::fromPool(nullptr,"HP:", fl);
		hp->setFontSize(16);
		hp->addOutline(r::Color::Black);
		hp->setTextColor(r::Color::Red);

		auto t0 = r2::Text::fromPool(nullptr, "#1 - Shotgun", fl);
		t0->setFontSize(16);
		t0->addOutline(r::Color::Black);
		t0->setTextColor(r::Color::Red);

		t0 = r2::Text::fromPool(nullptr, "#2 - Plasma Rifle", fl);
		t0->setFontSize(16);
		t0->addOutline(r::Color::Black);
		t0->setTextColor(r::Color::Red);

		fl->reflow();
		fl->syncAllMatrix();
	}
	else {
		hp->setText("HP:"s + std::to_string(game->player->hp));
	}
}

/*
void UI::drawRec(rs::GfxContext* ctx){
	auto sc = getScene();
	ctx->pushViewMatrix();
	ctx->loadViewMatrix(Matrix44::identity);
	rui::Canvas::drawRec(ctx);
	ctx->popViewMatrix();
}
*/
