#include "Enemy.h"

Enemy::Enemy(float x, float y, float w, float h) {
	rect = { x, y, w, h };
}

Enemy::~Enemy() {
	
}

void Enemy::simulate(float dt) {

}

SDL_FRect Enemy::getFRect() {
	return rect;
}