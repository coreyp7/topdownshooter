#include "GameState.h"

GameState::GameState(Player* player) {
	this->player = player;

	dt = 0;
	lastUpdate = 0;
	camera = { 0, 0, 1280, 720 };//@hardcoded: this should be set from main or
	//something; constructor params probably.
	qTree = new QuadTree(0, 0, 3840, 2160); //@hardcoded

	// Here I'm hardcoding test enemies.
	// Later, this would either:
	// - be spawned in dynamically like a hoarde type game
	// - loaded into a level ala a tile in the tilemap
	enemies.push_back(new Enemy(200, 200, 75, 75));
	enemies.push_back(new Enemy(300, 200, 75, 75));
	enemies.push_back(new Enemy(400, 200, 75, 75));
	entityIdMap.insert({ player->id, player });
	entityIdMap.insert({ enemies[0]->id, enemies[0] });
	entityIdMap.insert({ enemies[1]->id, enemies[1] });
	entityIdMap.insert({ enemies[2]->id, enemies[2] });
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

	// Update QuadTree (for now, deleting it and rebuilding it every frame).
	qTree->~QuadTree(); // @todo: have quadtree maintain itself
	qTree = new QuadTree(0, 0, 3840, 2160); // @hardcoded: !!!!!!!!!!!!!!!!
	qTree->insert(player);

	simulateEnemies();
	simulateProjectiles();

	resolveCollisions();

	lastUpdate = SDL_GetTicks();
}

//TODO: should change the name of this function. I'd name this something like
// 'processAndResolveCollisions'
void GameState::resolveCollisions() {
	for (int i = 0; i < enemies.size(); i++) {
		std::set<std::tuple<Uint16, Uint16>> collisions = qTree->getCollisionsWithEntity(enemies[i]);
		std::set<std::tuple<Uint16, Uint16>>::iterator itr;

		for(itr = collisions.begin(); itr != collisions.end(); itr++){
			std::tuple<Uint16, Uint16> currentCollisionIds = *itr;
			Uint16 entity1Id = std::get<0>(currentCollisionIds);
			Uint16 entity2Id = std::get<1>(currentCollisionIds);
			Entity* entity1 = getEntityById(entity1Id);
			Entity* entity2 = getEntityById(entity2Id);

			if (entity1 == nullptr || entity2 == nullptr) {
				// if either were invalid, then ignore iteration and log in console
				printf(
					"WARNING: either %i or %i return nullptr from id map. Cannot handle collision.\n",
					entity1Id, entity2Id);
				continue;
			}

			// TODO: send this to function for processing and resolving collision between both entities.
			if (entity1->getEntityType() == ENEMY && entity2->getEntityType() == PROJECTILE) {
				// resolve their collision
				printf("Enemy %i collided with projectile %i\n", entity1Id, entity2Id);
				
			}
		}
	}

	// TODO: fil collisions with ids obtained from quadtree work.

}

void GameState::simulateEnemies() {
	SDL_FRect qTreeRect = { 0, 0, 3840, 2160 };
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->simulate(dt, player->getPosition());
		qTree->insert(enemies[i]);
	}
}

void GameState::simulateProjectiles() {
	SDL_FRect qTreeRect = { 0, 0, 3840, 2160 };
	for (int i = 0; i < projectiles.size(); i++) {
		projectiles[i]->simulate(dt);
		if (!checkCollision(&qTreeRect, &projectiles[i]->pos)) {
			// Delete if its outside the quadtree (and thus, the entire level)
			printf("Deleted projectile at (%f,%f)\n", projectiles[i]->pos.x, projectiles[i]->pos.y);
			delete projectiles[i];
			projectiles.erase(projectiles.begin() + i);
			i--; // move our i back so that we don't skip a variable
			// @eh: could have this be done outside this loop, would maybe be clearer.
			// seems to be working, so leaving for now.
		}
		else {
			qTree->insert(projectiles[i]);

			// TODO: check for collisions with this
		}
	}
}

Player* GameState::getPlayer() {
	return player;
}

std::vector<Projectile*>* GameState::getProjectiles() {
	return &projectiles;
}

std::vector<Enemy*>* GameState::getEnemies() {
	return &enemies;
}

SDL_FRect GameState::getCamera() {
	return camera;
}

QuadTree* GameState::getQuadTree() {
	return qTree;
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

	// @refactor: could put this stuff into its own function
	// so if the id is invalid, it doesn't crash.
	entityIdMap.insert({ newProj->id, newProj });
	

	//qTree->insert(new)
}
Entity* GameState::getEntityById(Uint16 id) {
	auto entry = entityIdMap.find(id);
	
	if (entry != entityIdMap.end()) {
		return entry->second;
	}
	return nullptr;
}

bool GameState::checkCollision(SDL_FRect* entity1, SDL_FRect* entity2) {
	/*SDL_FRect rect1 = *entity1->rect;
	SDL_FRect rect2 = *entity2->rect;*/
	SDL_FRect rect1 = *entity1; // TODO: change this function to match the params instead of this shit
	SDL_FRect rect2 = *entity2;

	bool xCollision = (((rect1.x + rect1.w) >= (rect2.x)) && ((rect2.x + rect2.w) >= (rect1.x)));
	bool yCollision = (((rect1.y + rect1.h) >= (rect2.y)) && ((rect2.y + rect2.h) >= (rect1.y)));

	return xCollision && yCollision;
}

// All the specific private stuff will be down here.
