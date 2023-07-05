#pragma once
#include <SDL.h>
#include "Entity.h"
#include "EntityType.h"

class Enemy : public Entity {
public:
	SDL_FRect rect;
	const int MOVE_SPEED = 100; // @temporary, @testing

	Enemy(float x, float y, float w, float h);
	~Enemy();

	void simulate(float dt, SDL_FPoint playerPosition);

	// Entity stuff
	SDL_FRect* getFRect();
	SDL_FPoint getPosition();
	EntityType getEntityType();
};
