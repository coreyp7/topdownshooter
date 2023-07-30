#include "LargeEnemy.h"

LargeEnemy::LargeEnemy(float x, float y, int level) : Enemy(x,y,level) {
	rect.w = SIZE;
	rect.h = SIZE;
	hp = 100;
	maxHp = 100;
}

LargeEnemy::~LargeEnemy() {

}

int LargeEnemy::getEnemyType() {
	return 2;
}

void LargeEnemy::simulate(float dt, SDL_FPoint playerPosition) {
	rect.x += 250 * dt;
}
