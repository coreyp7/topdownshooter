#include "SmallEnemy.h"

SmallEnemy::SmallEnemy(float x, float y) : Enemy(x,y)
{
	rect.w = SIZE;
	rect.h = SIZE;
}


SmallEnemy::~SmallEnemy() {

}