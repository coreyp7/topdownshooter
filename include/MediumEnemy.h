#pragma once
#include "Enemy.h"

class MediumEnemy : public Enemy {
public:
	MediumEnemy(float x, float y);
	~MediumEnemy();

	void simulate(float dt, SDL_FPoint playerPosition);
	int getEnemyType();

	const int SIZE = 65;
	const int MOVE_SPEED = 60;

};