#pragma once
#include <SDL.h> // ONLY USED FOR SDL_Rect
#include "Direction.h"
#include "Entity.h"

class Player : public Entity
{
public:

	// @hardcoded: change these to be static and editable by imgui for testing
	const int MOVE_SPEED = 450;

	// @hardcoded: this should be a regular member that can change on powerup
	// pickup
	//const int PROJECTILE_SPEED = 500;
	const int PROJECTILE_SPEED = 1000;
	//const int PROJECTILE_SPEED = 5;
	const int HITBOX_RADIUS = 25;

	int xVel, yVel;

	SDL_FRect rect; // this is a hacky fix, needs to stay if this abides by Entity interface

	//SDL_FRect pos; // center of box? not sure yet
	SDL_FPoint pos;

	Player(SDL_FPoint pos);

	Uint32 invulTime;

	//int bulletSpeed = 500; // @idea: default speed, could be changed by powerup.

	void startMoving(Direction direction);
	void stopMoving(Direction direction);

	void simulate(float dt, SDL_FRect bounds);

	SDL_FRect* getFRect();
	SDL_FPoint getPosition();
	EntityType getEntityType();
};

