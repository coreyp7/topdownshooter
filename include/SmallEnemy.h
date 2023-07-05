#pragma once
#include "Enemy.h"

class SmallEnemy : public Enemy {
public:
	SmallEnemy(float x, float y);
	~SmallEnemy();

	const int MOVE_SPEED = 100;
	const float SIZE = 45;
};