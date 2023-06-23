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
	/* 
	1. get vector from center of player circle to the point (x/y params).
	2. normalize that vector into a unit vector
  3. multiply unit vector by radius.
  4. Add the multiplied unit vector to the player's position.

  This gives us the point to spawn the projectile at. 
	*/
	printf("Shoot bullet at (%i,%i)\n", x, y);
  //1
	float xVec, yVec;
	xVec = x - player->pos.x;
	yVec = y - player->pos.y;
  //2: normalize into unit vector
	float vectorMagnitude = sqrt( (xVec*xVec) + (yVec*yVec) );
	float xUnitVector = xVec / vectorMagnitude;
	float yUnitVector = yVec / vectorMagnitude;

  //3+4: this is the spawn point of the projectile.
  float circleEdgeX = player->pos.x + (xUnitVector * player->HITBOX_RADIUS);
  float circleEdgeY = player->pos.y + (yUnitVector * player->HITBOX_RADIUS);
  SDL_FPoint edgeSpawnPoint = {circleEdgeX, circleEdgeY}; 
  
  // Projectiles' velocity; just the unit vector multiplied by the projectile
  // speed.
  float xVel = xUnitVector * player->PROJECTILE_SPEED;
  float yVel = yUnitVector * player->PROJECTILE_SPEED;

	Projectile* newProj = new Projectile(edgeSpawnPoint, xVel, yVel);
	projectiles.push_back(newProj);


}

// All the specific private stuff will be down here.
