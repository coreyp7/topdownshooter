#include "EnemyManager.h"
#include "GameState.h"

EnemyManager::EnemyManager(GameState* gameState) {
	this->gameState = gameState;
}


EnemyManager::~EnemyManager() {

}

void EnemyManager::shootEnemyProjectile(float x, float y, float xVel, float yVel) {
	gameState->shootEnemyProjectile(x, y, xVel, yVel);
}