#include "Projectile.h"

Projectile::Projectile(SDL_FRect pos, float xVel, float yVel) {
	this->xVel = xVel;
	this->yVel = yVel;
	this->pos = pos;
}

Projectile::~Projectile() {

}

void Projectile::simulate(float dt) {
	pos.x += xVel * dt;
	pos.y += yVel * dt;
}