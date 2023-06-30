#pragma once
#include <SDL.h>
class Entity
{
public:
	//virtual int getId() = 0;

	virtual SDL_FRect getFRect() = 0;

	//TODO: add a getType() for obtaining type of entity (player, projectile, enemy, etc.)
};

