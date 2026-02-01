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
	x = sc->cameraPos.x;
	y = sc->cameraPos.y;
	trsDirty = true;
	syncAllMatrix();

	if(hp == 0){
		hp = r2::Text::fromPool(nullptr,"HP:", this);
		hp->centered();
		hp->x = 32;
		hp->y = 32;
		hp->trsDirty = true;
		hp->addOutline(r::Color::Black);
		hp->setTextColor(r::Color::Red);
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
