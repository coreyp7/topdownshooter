#pragma once
#include <SDL.h>
#include "Player.h"

class GameState {

public:
	GameState(Player* player);
	~GameState();

	// Simulate entire game state for dt seconds.
	void simulate();

	Player* getPlayer();

private:

	Player* player;
	float dt;
	float lastUpdate;

};