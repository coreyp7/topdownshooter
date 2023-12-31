#include "SmallEnemy.h"

SmallEnemy::SmallEnemy(float x, float y) : Enemy(x,y)
{
	rect.w = SIZE;
	rect.h = SIZE;
	hp = 3;
	maxHp = 3;
}

SmallEnemy::SmallEnemy(float x, float y, int level) : Enemy(x,y,level) {
	rect.w = SIZE;
	rect.h = SIZE;
	hp = 2 + level;
	maxHp = hp;
}

SmallEnemy::~SmallEnemy() {

}

void SmallEnemy::simulate(float dt, SDL_FPoint playerPosition) {
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

	float amp = level * .75;

	if (level > 1) {
		xVel *= amp;
		yVel *= amp;

		if (vectorMagnitude < (85 * level-1)) {
			xVel *= 2;
			yVel *= 2;
		}
	}

	rect.x += xVel * dt;
	rect.y += yVel * dt;
}

int SmallEnemy::getEnemyType() {
	return 0;
}