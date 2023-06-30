#include "Enemy.h"

Enemy::Enemy(float x, float y, float w, float h) {
	rect = { x, y, w, h };
}

Enemy::~Enemy() {
	
}

// TODO: this should be moved to a file dedicated to vector space operations.
void Enemy::simulate(float dt, SDL_FPoint playerPosition) {
	// Change position to be in world space instead of relative to window.
	/*int xWorldPos = x + camera.x;
	int yWorldPos = y + camera.y;*/
	SDL_FPoint ourPosition = this->getPosition();

	//1
	float xVec, yVec;
	/*xVec = xWorldPos - player->pos.x;
	yVec = yWorldPos - player->pos.y;*/
	/*xVec = ourPosition.x - playerPosition.x;
	yVec = ourPosition.y - playerPosition.y;*/
	xVec = playerPosition.x - ourPosition.x;
	yVec = playerPosition.y - ourPosition.y;

	//2: normalize into unit vector
	float vectorMagnitude = sqrt((xVec * xVec) + (yVec * yVec));
	float xUnitVector = xVec / vectorMagnitude;
	float yUnitVector = yVec / vectorMagnitude;

	//3+4: this is the spawn point of the projectile.
	/*float circleEdgeX = player->pos.x + (xUnitVector * player->HITBOX_RADIUS);
	float circleEdgeY = player->pos.y + (yUnitVector * player->HITBOX_RADIUS);
	SDL_FPoint edgeSpawnPoint = { circleEdgeX, circleEdgeY };*/

	// Projectiles' velocity; just the unit vector multiplied by the projectile
	// speed.
	float xVel = xUnitVector * MOVE_SPEED;
	float yVel = yUnitVector * MOVE_SPEED;

	/*Projectile* newProj = new Projectile(edgeSpawnPoint, xVel, yVel);
	projectiles.push_back(newProj);*/
	rect.x += xVel * dt;
	rect.y += yVel * dt;
}

SDL_FRect Enemy::getFRect() {
	return rect;
}

SDL_FPoint Enemy::getPosition() {
	return SDL_FPoint{ rect.x + rect.w / 2, rect.y + rect.h / 2 };
}

EntityType Enemy::getEntityType() {
	return ENEMY;
}