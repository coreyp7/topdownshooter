#pragma once
#include <SDL.h>
#include "Entity.h"
#include "EntityType.h"

class Enemy : public Entity {
public:
	SDL_FRect rect;
	int level;
	Uint32 spawnTime;
	//const int MOVE_SPEED = 100; // @temporary, @testing
	//const int MOVE_SPEED = 50; // @temporary, @testing
	//EnemyManager* manager;

	Enemy(float x, float y);
	Enemy(float x, float y, int level);
	~Enemy();

	virtual void simulate(float dt, SDL_FPoint playerPosition) = 0;

	// 0 = smallenemy
	// 1 = medium enemy
	// 2 = large enemy
	virtual int getEnemyType() = 0;

	// Entity stuff
	SDL_FRect* getFRect();
	SDL_FPoint getPosition();
	EntityType getEntityType();
};
