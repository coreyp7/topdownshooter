#include "Projectile.h"

Projectile::Projectile(SDL_FPoint pos, float xVel, float yVel, float size) {
	this->xVel = xVel;
	this->yVel = yVel;
	this->size = size;
	this->pos = SDL_FRect{ pos.x - size / 2, pos.y - size / 2, size, size };
	typeOfProjectile = PROJECTILE;
}

Projectile::~Projectile() {

}

void Projectile::simulate(float dt) {
	pos.x += xVel * dt;
	pos.y += yVel * dt;
}

SDL_FRect* Projectile::getFRect() {
	return &this->pos;
}

SDL_FPoint Projectile::getPosition() {
	return SDL_FPoint{ pos.x + size / 2, pos.y + size / 2 };
}

EntityType Projectile::getEntityType() {
	return typeOfProjectile;
}
