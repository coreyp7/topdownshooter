#include "MediumEnemy.h"

MediumEnemy::MediumEnemy(float x, float y) : Enemy(x, y) {
	rect.w = SIZE;
	rect.h = SIZE+20;
	hp = 9;
	maxHp = 9;
}

MediumEnemy::~MediumEnemy() {

}

void MediumEnemy::simulate(float dt, SDL_FPoint playerPosition) {
	SDL_FPoint ourPosition = this->getPosition();

	//1
	float xVec, yVec;
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

	if (vectorMagnitude < 250) {
		xVel *= 2;
		yVel *= 2;
	}

	rect.x += xVel * dt;
	rect.y += yVel * dt;
}

int MediumEnemy::getEnemyType() {
	return 1;
}