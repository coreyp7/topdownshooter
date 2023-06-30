#include "Player.h"
#include <stdio.h>

Player::Player(SDL_FPoint pos) {
	this->pos = pos;
	xVel = 0;
	yVel = 0;
}

void Player::startMoving(Direction direction) {
	switch (direction) {
	case UP:
		yVel = -MOVE_SPEED;
		break;
	case DOWN:
		yVel = MOVE_SPEED;
		break;
	case LEFT:
		xVel = -MOVE_SPEED;
		break;
	case RIGHT:
		xVel = MOVE_SPEED;
		break;
	}
}

void Player::stopMoving(Direction direction) {
	switch (direction) {
	case UP:
		yVel = 0;
		break;
	case DOWN:
		yVel = 0;
		break;
	case LEFT:
		xVel = 0;
		break;
	case RIGHT:
		xVel = 0;
		break;
	}
}

void Player::simulate(float dt) {
	pos.x += xVel * dt;
	pos.y += yVel * dt;
}

SDL_FRect Player::getFRect() {
	SDL_FRect rect = {pos.x - HITBOX_RADIUS, pos.y - HITBOX_RADIUS, HITBOX_RADIUS*2, HITBOX_RADIUS*2};
	return rect;
}

SDL_FPoint Player::getPosition() {
	return pos;
}