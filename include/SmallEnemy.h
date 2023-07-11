#pragma once
#include "Enemy.h"
#include "EnemyManager.h"

class SmallEnemy : public Enemy {
public:
	SmallEnemy(float x, float y);
	~SmallEnemy();

	void simulate(float dt, SDL_FPoint playerPosition);
	int getEnemyType();

	const int MOVE_SPEED = 100; // @change: maybe this should escalate as the waves increment
	const float SIZE = 45;
};