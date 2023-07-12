#include "EnemyManager.h"

void shootProjectile(float x, float y, float xVel, float yVel, int size) {
	SDL_FPoint position = { x, y };

	Projectile* newProj = new Projectile(position, xVel, yVel, size);

	// TODO: just have this be a value in the constructor, make creating
	// projectiles in a more general function for player/enemies.
	newProj->typeOfProjectile = ENEMY_PROJECTILE;
	newProj->hp = 3;
	newProj->maxHp = 3;
	addEntity(newProj);
}
