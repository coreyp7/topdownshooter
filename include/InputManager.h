#pragma once
#include <SDL.h>
#include <vector>
#include "Player.h"
/**
Handles SDL events and calls functions in the Player apprpriate
to the proccessed user inputs.
*/
class InputManager
{
public:
	InputManager(Player* player);
	void handleEvent(SDL_Event event);

private:
	std::vector<SDL_Event> events;
	Player* player;
};

