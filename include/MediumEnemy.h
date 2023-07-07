#pragma once
#include "Enemy.h"

class MediumEnemy : public Enemy {
public:
	MediumEnemy(EnemyManager* manager, float x, float y);
	~MediumEnemy();

	void simulate(float dt, SDL_FPoint playerPosition);
	int getEnemyType();

	const int SIZE = 65;
	const int MOVE_SPEED = 60;

	Uint32 nextShotTicks;
	const int nextShotDelay = 1000;

};