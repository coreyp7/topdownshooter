#pragma once
#include <SDL.h>
class GameState;

class EnemyManager {
public:
	GameState* gameState;

	EnemyManager(GameState* gameState);
	~EnemyManager();

	void shootEnemyProjectile(float x, float y, float xVel, float yVel, int size);
};