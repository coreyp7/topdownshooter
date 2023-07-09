#pragma once
#include <SDL.h>
#include <vector>
#include "GameState.h"

/**
Handles SDL events and calls functions in the Player apprpriate
to the proccessed user inputs.
*/
//class InputManager
//{
//public:
	//InputManager(GameState* gameState);
void handleEvent(SDL_Event* event, Player* player);

//private:
//std::vector<SDL_Event> events;

	// NOTE: this should be the GameState instead.
//GameState* gameState;

void handleMovementEvent(SDL_Event* event, Player* player);

//};

