#include "EnemyManager.h"

void shootProjectile(float x, float y, float xVel, float yVel, int size) {
	SDL_FPoint position = { x, y };

	Projectile* newProj = new Projectile(position, xVel, yVel, size);

	// TODO: just have this be a value in the constructor, make creating
	// projectiles in a more general function for player/enemies.
	newProj->typeOfProjectile = ENEMY_PROJECTILE;
	addEntity(newProj);
}

//#include "GameState.h"
//
//EnemyManager::EnemyManager(GameState* gameState) {
//	//this->gameState = gameState;
//}
//
//
//EnemyManager::~EnemyManager() {
//
//}
//
//void EnemyManager::shootEnemyProjectile(float x, float y, float xVel, float yVel, int size) {
//	//gameState->shootEnemyProjectile(x, y, xVel, yVel, size);
//}