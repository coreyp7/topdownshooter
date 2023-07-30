#pragma once
#include "Enemy.h"
#include "EnemyManager.h"

class MediumEnemy : public Enemy {
public:
	MediumEnemy(float x, float y, int level);
	~MediumEnemy();

	void simulate(float dt, SDL_FPoint playerPosition);
	int getEnemyType();

	const int SIZE = 65;
	const int MOVE_SPEED = 60;
	const int PROJECTILE_SPEED = 300;
	const int PROJECTILE_SIZE = 40;

	Uint32 nextShotTicks;
	const int nextShotDelay = 1000;

};