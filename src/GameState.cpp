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
	float xVec, yVec;
	xVec = x - player->pos.x;
	yVec = y - player->pos.y;

  float vectorMagnitude = sqrt( (xVec*xVec) + (yVec*yVec) );
  float xUnitVector = xVec / vectorMagnitude;
  float yUnitVector = yVec / vectorMagnitude;
  xUnitVector *= player->PROJECTILE_SPEED;
  yUnitVector *= player->PROJECTILE_SPEED;

	//Projectile* newProj = new Projectile(player->pos, xVel, yVel);
	Projectile* newProj = new Projectile(player->pos, xUnitVector, yUnitVector);
	newProj->pos.w = 5;
	newProj->pos.h = 5;
	projectiles.push_back(newProj);
}

// All the specific private stuff will be down here.
