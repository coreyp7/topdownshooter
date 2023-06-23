#include "Projectile.h"

Projectile::Projectile(SDL_FPoint pos, float xVel, float yVel) {
	this->xVel = xVel;
	this->yVel = yVel;
	this->pos = SDL_FRect{pos.x, pos.y, 15, 15};
}

Projectile::~Projectile() {

}

void Projectile::simulate(float dt) {
	pos.x += xVel * dt;
	pos.y += yVel * dt;
}