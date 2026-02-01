#include "stdafx.h"
#include "all.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "BulMan.hpp"
#include "App_GameState.h"

Player::Player(Game*g,r2::Node* parent) : Entity(g,parent){

}

void Player::update(double dt) {
	Entity::update(dt);
}

void Player::controlsMove(vec2 move, double dt) {
	vec2 norm = move.getNormalizedSafeZero();
	float speed = data->speed;
	dx += norm.x * dt * speed;
	dy += norm.y * dt * speed;
}

void Player::controls(double dt){
	vec2 move;
	if (rs::Input::isPressed(Pasta::Key::KB_Q)) {
		move += { -1, 0 };
	}
	if (rs::Input::isPressed(Pasta::Key::KB_Z)) {
		move+={ 0,-1 };
	}
	if (rs::Input::isPressed(Pasta::Key::KB_S)) {
		move+={ 0,1 };
	}
	if (rs::Input::isPressed(Pasta::Key::KB_D)) {
		move+={ 1,0 };
	}

	if (rs::Input::isPressed(Pasta::Key::KB_1)) 
		weapon = "shotgun";
	if (rs::Input::isPressed(Pasta::Key::KB_2)) 
		weapon = "plasma";
	if (rs::Input::isPressed(Pasta::Key::KB_2)) 
		weapon = "rocket";

	if(move!=vec2(0,0))
		controlsMove(move,dt);

	if (rs::Input::isMouseJustPressed(Pasta::Key::MOUSE_LEFT)) {
		auto mousePos = game->getGameMousePos();
		fire(mousePos.x, mousePos.y);
	}
}

void Player::fire(int pixX, int pixY){
	auto proj = Data::getProj(weapon.c_str());

	auto rnd = rd::Rand::get();
	if( proj->nb == 1)
		Super::fire(pixX, pixY);
	else 
	{
		for (int i = 0; i < proj->nb; ++i) {
			int nx = pixX + rnd.dice(-12, 12);
			int ny = pixY + rnd.dice(-12, 12);
			Super::fire( nx, ny, proj);
		}
	}
}

