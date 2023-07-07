#include "Enemy.h"

Enemy::Enemy(float x, float y) {
	rect = { x, y, 0, 0 }; // width/height set in subclasses
}

Enemy::~Enemy() {
	
}

SDL_FRect* Enemy::getFRect() {
	return &rect;
}

SDL_FPoint Enemy::getPosition() {
	return SDL_FPoint{ rect.x + rect.w / 2, rect.y + rect.h / 2 };
}

EntityType Enemy::getEntityType() {
	return ENEMY;
}


//// TODO: this should be a virtual method which is implemented in subclasses.
//void Enemy::simulate(float dt, SDL_FPoint playerPosition) {
//	SDL_FPoint ourPosition = this->getPosition();
//
//	//1
//	float xVec, yVec;
//	xVec = playerPosition.x - ourPosition.x;
//	yVec = playerPosition.y - ourPosition.y;
//
//	//2: normalize into unit vector
//	float vectorMagnitude = sqrt((xVec * xVec) + (yVec * yVec));
//	float xUnitVector = xVec / vectorMagnitude;
//	float yUnitVector = yVec / vectorMagnitude;
//
//	//3+4: this is the spawn point of the projectile.
//	/*float circleEdgeX = player->pos.x + (xUnitVector * player->HITBOX_RADIUS);
//	float circleEdgeY = player->pos.y + (yUnitVector * player->HITBOX_RADIUS);
//	SDL_FPoint edgeSpawnPoint = { circleEdgeX, circleEdgeY };*/
//
//	// Projectiles' velocity; just the unit vector multiplied by the projectile
//	// speed.
//	float xVel = xUnitVector * MOVE_SPEED;
//	float yVel = yUnitVector * MOVE_SPEED;
//
//	if (vectorMagnitude < 250) {
//		xVel *= 2;
//		yVel *= 2;
//	}
//
//	rect.x += xVel * dt;
//	rect.y += yVel * dt;
//}