#pragma once
#include <SDL.h> // ONLY USED FOR SDL_Rect
#include "Direction.h"

class Player
{
public:

	const int MOVE_SPEED = 450;

	int xVel, yVel;

	SDL_FRect pos; // center of box? not sure yet


	Player(SDL_FRect pos);

	void startMoving(Direction direction);
	void stopMoving(Direction direction);

	void simulate(float dt); // TODO pass dt to this
};

