#include "GameState.h"

GameState::GameState(Player* player) {
	this->player = player;

	dt = 0;
	lastUpdate = 0;
	camera = { 0, 0, 1280, 720 };//@hardcoded: this should be set from main or
	//something; constructor params probably.
}

GameState::~GameState() {
	this->player = nullptr;
}

// Entire state simulate which is public.
void GameState::simulate() {
	dt = (SDL_GetTicks() - lastUpdate) / 1000.f;

	// Simulate entire game for dt seconds.
	player->simulate(dt);
	moveCameraWithPlayer();

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

SDL_FRect GameState::getCamera() {
	return camera;
}

void GameState::moveCameraWithPlayer() {
	int offset = 150;

	// x axis
	if (player->pos.x > camera.x + camera.w - offset) {
		float newRight;
		newRight = player->pos.x + offset;
		camera.x = newRight - camera.w;
	}
	else if (player->pos.x < camera.x + offset) {
		float newLeft;
		newLeft = player->pos.x - offset;
		camera.x = newLeft;
	}

	// y axis
	if (player->pos.y > camera.y + camera.h - offset) {
		// bottom
		float newBottom;
		newBottom = player->pos.y + offset;
		camera.y = newBottom - camera.h;
	}
	else if (player->pos.y < camera.y + offset) {
		// top
		float newTop;
		newTop = player->pos.y - offset;
		camera.y = newTop;
	}
}

// Param should be the position in the window the player clicked.
// This function will shoot it in worldspace (using camera). 
// Don't worry about camera shit when calling this.
void GameState::playerShootBullet(int x, int y) {
	/*
	1. get vector from center of player circle to the point (x/y params).
	2. normalize that vector into a unit vector
  3. multiply unit vector by radius.
  4. Add the multiplied unit vector to the player's position.
  This gives us the point to spawn the projectile at.
	*/

	// Change position to be in world space instead of relative to window.
	int xWorldPos = x + camera.x;
	int yWorldPos = y + camera.y;

	printf("Shoot bullet at (%i,%i) in global worldspace.\n", xWorldPos, yWorldPos);
	//1
	float xVec, yVec;
	xVec = xWorldPos - player->pos.x;
	yVec = yWorldPos - player->pos.y;

	//2: normalize into unit vector
	float vectorMagnitude = sqrt((xVec * xVec) + (yVec * yVec));
	float xUnitVector = xVec / vectorMagnitude;
	float yUnitVector = yVec / vectorMagnitude;

	//3+4: this is the spawn point of the projectile.
	float circleEdgeX = player->pos.x + (xUnitVector * player->HITBOX_RADIUS);
	float circleEdgeY = player->pos.y + (yUnitVector * player->HITBOX_RADIUS);
	SDL_FPoint edgeSpawnPoint = { circleEdgeX, circleEdgeY };

	// Projectiles' velocity; just the unit vector multiplied by the projectile
	// speed.
	float xVel = xUnitVector * player->PROJECTILE_SPEED;
	float yVel = yUnitVector * player->PROJECTILE_SPEED;

	Projectile* newProj = new Projectile(edgeSpawnPoint, xVel, yVel);
	projectiles.push_back(newProj);
}

// All the specific private stuff will be down here.
