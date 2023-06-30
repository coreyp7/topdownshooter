#include "Projectile.h"

Projectile::Projectile(SDL_FPoint pos, float xVel, float yVel) {
	this->xVel = xVel;
	this->yVel = yVel;
	this->pos = SDL_FRect{ pos.x - SIZE / 2, pos.y - SIZE / 2, SIZE, SIZE };
}

Projectile::~Projectile() {

}

void Projectile::simulate(float dt) {
	pos.x += xVel * dt;
	pos.y += yVel * dt;
}

SDL_FRect Projectile::getFRect() {
	return this->pos;
}

SDL_FPoint Projectile::getPosition() {
	return SDL_FPoint{ pos.x + SIZE / 2, pos.y + SIZE / 2 };
}

EntityType Projectile::getEntityType() {
	return PROJECTILE;
}
