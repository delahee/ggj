#include "stdafx.h"
#include "all.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "BulMan.hpp"

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

	if (rs::Input::isJustPressed(Pasta::Key::MOUSE_LEFT)) {
		fire();
	}
}

void Player::fire(){
	Bullet b;
	auto ppos = getPixelPos();
	b.x = ppos.x;
	b.y = ppos.y;
	b.sprName = "bullet";

	float speed = 10.0f;
	if (dy && dx) {
		vec2 dir = { dx,dy };
		dir = dir.getNormalizedSafeZero();
		b.dx = dx * speed;
		b.dy = dy * speed;
	}
	else 
		b.dy = 1 * speed;
	b.life = 10.0f;
	b.frictx = b.fricty = 1;
	game->bulMan->addBullet(b);
}
