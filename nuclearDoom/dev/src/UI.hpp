#pragma once

#include "rui/Canvas.hpp"

class Game;
class UI : public rui::Canvas{
	typedef rui::Canvas Super;
public:
	Game* game{};
	UI(Game*game,r2::Node * parent);

	r::Matrix44 backup;
	//virtual void update(double dt) override;
	//virtual void drawRec(rs::GfxContext* ctx) override;
	//virtual bool drawPrepare(rs::GfxContext* ctx);
	//virtual bool drawCleanup(rs::GfxContext* ctx);

	r2::Text* hp{};
	virtual void update(double dt ) override;
	//void updateLife();
};