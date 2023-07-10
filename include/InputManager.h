#pragma once
#include <SDL.h>
#include <vector>
#include "GameState.h"

// Module for reading input from user and updating the 
// game state based on it.

void handleEvent(SDL_Event* event, Player* player);

void handleMovementEvent(SDL_Event* event, Player* player);
