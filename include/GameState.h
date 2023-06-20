#pragma once
#include <SDL.h>
#include "Player.h"
#include "Projectile.h"
#include <stdio.h>
#include <vector>
#include <cmath>

class GameState {

public:
	GameState(Player* player);
	~GameState();

	// Simulate entire game state for dt seconds.
	void simulate();

	Player* getPlayer();
	std::vector<Projectile*>* getProjectiles();

	void playerShootBullet(int x, int y);

private:

	Player* player;
	float dt;
	float lastUpdate;
	std::vector<Projectile*> projectiles;

};
