#pragma once
#include "Enemy.h"

class LargeEnemy : public Enemy {
public:
	int SIZE = 150;

	LargeEnemy(float x, float y, int level);
	~LargeEnemy();

	void simulate(float dt, SDL_FPoint playerPosition);

	int getEnemyType();
};