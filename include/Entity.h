#pragma once
#include <SDL.h>
#include "EntityType.h"
class Entity
{
public:
	//static Uint16 ID_COUNTER; // this really needs to go somewhere else
	Uint16 id;

	Entity();
	~Entity();

	virtual EntityType getEntityType() = 0;

	virtual SDL_FRect* getFRect() = 0; // TODO: this should return a pointer

	// Return center point of this entity.
	virtual SDL_FPoint getPosition() = 0;

	//TODO: add a getType() for obtaining type of entity (player, projectile, enemy, etc.)
};

