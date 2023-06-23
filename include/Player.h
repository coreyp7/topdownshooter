#pragma once
#include <SDL.h> // ONLY USED FOR SDL_Rect
#include "Direction.h"

class Player
{
public:

  // @hardcoded: change these to be static and editable by imgui for testing
	const int MOVE_SPEED = 450;
  const int PROJECTILE_SPEED = 500;//this should just be regular member in
                                   //player object, so we can change on powerup
                                   //pickups, weapons, etc.
  const int HITBOX_RADIUS = 25;

	int xVel, yVel;

	//SDL_FRect pos; // center of box? not sure yet
	SDL_FPoint pos;

	Player(SDL_FPoint pos);

  //int bulletSpeed = 500; // @idea: default speed, could be changed by powerup.

	void startMoving(Direction direction);
	void stopMoving(Direction direction);

	void simulate(float dt); // TODO pass dt to this
};

