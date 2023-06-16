#pragma once
#include <SDL.h> // ONLY USED FOR SDL_Rect
#include "Direction.h"

class Player
{
public:

	const int MOVE_SPEED = 5;

	int xVel, yVel;

	SDL_Rect pos; // center of box? not sure yet


	Player(SDL_Rect pos);

	void startMoving(Direction direction);
	void stopMoving(Direction direction);

	void simulate(); // TODO pass dt to this
};

