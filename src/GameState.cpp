#include "GameState.h"

GameState::GameState(Player* player) {
	this->player = player;

	dt = 0;
	lastUpdate = 0;
}

GameState::~GameState() {
	this->player = nullptr;
}

// Entire state simulate which is public.
void GameState::simulate() {
	dt = (SDL_GetTicks() - lastUpdate) / 1000.f;

	// Simulate entire game for dt seconds.
	player->simulate(dt);
	for (int i = 0; i < projectiles.size(); i++) {
		projectiles[i]->simulate(dt);
	}

	lastUpdate = SDL_GetTicks();
}

Player* GameState::getPlayer() {
	return player;
}

std::vector<Projectile*>* GameState::getProjectiles() {
	return &projectiles;
}

void GameState::playerShootBullet(int x, int y) {
	printf("Shoot bullet at (%i,%i)\n", x, y);
	float xVel, yVel;
	xVel = x - player->pos.x;
	yVel = y - player->pos.y;

	Projectile* newProj = new Projectile(player->pos, xVel, yVel);
	projectiles.push_back(newProj);
}

// All the specific private stuff will be down here.