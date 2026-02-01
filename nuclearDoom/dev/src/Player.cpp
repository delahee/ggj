#include "stdafx.h"
#include "all.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "BulMan.hpp"
#include "App_GameState.h"

Player::Player(Game*g,r2::Node* parent) : Entity(g,parent){

}

void Player::update(double dt) {
	updateMovement(dt);
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

	if(move!=vec2(0,0))
		controlsMove(move,dt);

	if (rs::Input::isMouseJustPressed(Pasta::Key::MOUSE_LEFT)) {
		auto mousePos = game->getGameMousePos();
		fire(mousePos.x, mousePos.y);
	}
}

void Player::fire(int pixX, int pixY){
	Bullet b;
	auto ppos = getPixelPos();
	b.x = ppos.x;
	b.y = ppos.y;
	b.sprName = "bullet";

	float speed = 150.0f;

	vec2 dir = { pixX - b.x, pixY - b.y };
	dir = dir.getNormalizedSafeZero();
	b.dx = dir.x * speed;
	b.dy = dir.y * speed;

	if( !b.dx &&!b.dy){
		b.dy = 1;
	}
	b.life = 10.0f;
	b.frictx = b.fricty = 1;
	game->bulMan->addBullet(b);
}
