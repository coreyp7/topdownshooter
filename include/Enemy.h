#pragma once
#include <SDL.h>
#include "Entity.h"

class Enemy : public Entity {
public:
	SDL_FRect rect;

	Enemy(float x, float y, float w, float h);
	~Enemy();

	void simulate(float dt);

	// Entity stuff
	SDL_FRect getFRect();
};
