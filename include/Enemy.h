#pragma once
#include <SDL.h>
#include "Entity.h"
#include "EntityType.h"

class Enemy : public Entity {
public:
	SDL_FRect rect;
	//const int MOVE_SPEED = 100; // @temporary, @testing
	//const int MOVE_SPEED = 50; // @temporary, @testing

	Enemy(float x, float y);
	~Enemy();

	virtual void simulate(float dt, SDL_FPoint playerPosition) = 0;

	// 0 = smallenemy
	// 1 = medium enemy
	virtual int getEnemyType() = 0;

	// Entity stuff
	SDL_FRect* getFRect();
	SDL_FPoint getPosition();
	EntityType getEntityType();
};
