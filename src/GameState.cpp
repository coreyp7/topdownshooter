#include "GameState.h"

#define WORLD_SIZE 1500

// Here's stuff that are only visible in this file.
void moveCameraWithPlayer(Player* player);

bool checkCollision(SDL_FRect* entity1, SDL_FRect* entity2);
void resolveCollisions();
int resolveEntityCollision(Entity* entity1, Entity* entity2);

void simulateProjectiles();
void simulateEnemies();
void addEntity(Entity* entity);

Entity* getEntityById(Uint16 id);
void removeEntity(Entity* entity);


// Marks how many enemies are in a spawn file.
// May have to change to handle in loadSpawnFile, so we can have any
// different number of enemies.
int HARDCODED_SPAWN_COUNT = 3;

// externs
Player player = Player(SDL_FPoint{ 250, 250 });
SDL_FRect camera = { 0, 0, 1280, 720 };
std::vector<Entity*> entities; // TODO: change to fixed sized array maybe
std::unordered_map<Uint16, Entity*> entityIdMap;
QuadTree* qTree = new QuadTree(0, 0, WORLD_SIZE, WORLD_SIZE);

// for imgui
int enemySpawnLevel = 1;

// normal shit
float dt = 0;
Uint32 lastUpdate = 0;

// getting spawning / waves shit done
std::queue<Enemy*> enemySpawnList;
int wave = 1;
Uint32 currentWaveStartTicks;
Uint32 currentWaveTicks; // how long this wave has been going on

//bool gameOver = false;
Uint32 gameOverTicks = 0;

bool gameOver() {
	if (gameOverTicks != 0) {
		if ((gameOverTicks + 1000) < SDL_GetTicks()) {
			return true;
		}
	}
	return false;
}

void setupGameState() {
	entityIdMap.insert({ player.id, &player });

	//loadSpawnFile(1); // load first wave queue
}

void simulateWorld() {
	currentWaveTicks = SDL_GetTicks() - currentWaveStartTicks;

	// This approach to handling time is appropriate for this game.
	// Physics is simple and there's no networking.
	// If it becomes too inconsistent or gross later, then I will attempt
	// to refactor into example on 131/132 of 'Game Programming Patterns'.
	dt = (SDL_GetTicks() - lastUpdate) / 1000.f;
	lastUpdate = SDL_GetTicks();

	// Simulate entire game for dt seconds.
	SDL_FRect playerArea = { 0, 0, WORLD_SIZE, WORLD_SIZE };
	player.simulate(dt, playerArea);
	moveCameraWithPlayer(&player);

	// Update QuadTree (for now, deleting it and rebuilding it every frame).
	qTree->~QuadTree(); // @todo: have quadtree maintain itself (this is hard)
	qTree = new QuadTree(-1000, -1000, 3000, 3000); // @hardcoded
	qTree->insert(&player);

	// Spawn enemies which are ready to be spawned
	// TODO: make this behavior more complicated just incase we go over
	// the spawn time of an enemy.
	if (!enemySpawnList.empty()) {
		if (enemySpawnList.front()->spawnTime < currentWaveTicks) {
			// spawn it and remove from vec
			addEntity(enemySpawnList.front());
			enemySpawnList.pop();
			printf("Spawned enemy.\n");
		}
	}

	simulateEnemies();
	simulateProjectiles();

	resolveCollisions();

	bool noMoreEnemies = true;

	// @lazy fix that adds extra O(n). delete stuff thats dead.
	// If I ever start having performance problems, I'm going
	// to change these to contiguous arrays.
	for (int i = 0; i < entities.size(); i++) {
		Entity* curr = entities[i];

		// Mark if there's an enemy still alive, used for determining
		// if the wave is complete or not.
		if (curr->getEntityType() == ENEMY) {
			noMoreEnemies = false;
		}

		if (curr->dead) {
			entities.erase(entities.begin() + i);
			entityIdMap.erase(curr->id);
			qTree->remove(curr);
			delete curr;
			curr = nullptr;
			i--;

		}
	}

	if (enemySpawnList.empty() && noMoreEnemies) {
		// wave complete, start next wave
		wave++;
		loadSpawnFile(wave);
	}
}

// Goes through every entity and finds if it collides with anything.
// When collisions occur, they will be resolved.
void resolveCollisions() { //TODO: optimize; we don't need to be doing this for all the projectiles. Only for player & enemies.
	for (int i = 0; i < entities.size(); i++) {
		std::set<std::tuple<Uint16, Uint16>> collisions = qTree->getCollisionsWithEntity(entities[i]);
		std::set<std::tuple<Uint16, Uint16>>::iterator itr;
		
		for(itr = collisions.begin(); itr != collisions.end(); itr++){
			std::tuple<Uint16, Uint16> currentCollisionIds = *itr;
			Uint16 entity1Id = std::get<0>(currentCollisionIds);
			Uint16 entity2Id = std::get<1>(currentCollisionIds);
			Entity* entity1 = getEntityById(entity1Id);
			Entity* entity2 = getEntityById(entity2Id);

			resolveEntityCollision(entity1, entity2);
			/*if (entities[i]->dead) {
				entitiesToDestroy.push_back(i);
			}*/
		}
	}

	/*Entity* deadEntity;
	for (int i = 0; i < entitiesToDestroy.size(); i++) {
		deadEntity = entities[entitiesToDestroy[i]];
		entityIdMap.erase(deadEntity->id);
		qTree->remove(deadEntity);
		entities.erase(entities.begin() + entitiesToDestroy[i]);
		delete deadEntity;
		deadEntity = nullptr;
	}*/
}

void simulateEnemies() {
	SDL_FRect qTreeRect = { 0, 0, WORLD_SIZE, WORLD_SIZE }; // @hardcoded
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->getEntityType() == ENEMY) {
			Enemy* enemy = (Enemy*)entities[i];
			enemy->simulate(dt, player.getPosition());
			qTree->insert(entities[i]);
		}
	}
}

void simulateProjectiles() {
	SDL_FRect qTreeRect = { -1000, -1000, 3000, 3000 }; // @hardcoded

	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->getEntityType() != PROJECTILE && entities[i]->getEntityType() != ENEMY_PROJECTILE) continue;

		Projectile* projectile = (Projectile*)entities[i];
		projectile->simulate(dt);
		if (!checkCollision(&qTreeRect, &projectile->pos)) {
			// Delete if its outside the quadtree (and thus, the entire level)
			printf("Deleted projectile at (%f,%f)\n", projectile->pos.x, projectile->pos.y);
			projectile->dead = true;
		}
		else {
			qTree->insert(projectile);
		}
	}
}

Player* getPlayer() {
	return &player;
}

std::vector<Projectile*> getProjectiles() {
	//return &projectiles;
	std::vector<Projectile*> projectiles;
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->getEntityType() == PROJECTILE || entities[i]->getEntityType() == ENEMY_PROJECTILE) {
			Projectile* projectile = (Projectile*)entities[i];
			projectiles.push_back(projectile);
		}
	}
	return projectiles;
}

std::vector<Enemy*> getEnemies() {
	std::vector<Enemy*> enemies;
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->getEntityType() == ENEMY) {
			Enemy* enemy = (Enemy*)entities[i];
			enemies.push_back(enemy);
		}
	}
	return enemies;
}

SDL_FRect getCamera() {
	return camera;
}

QuadTree* getQuadTree() {
	return qTree;
}

void moveCameraWithPlayer(Player* player) {
	int xoffset = 450;
	int yoffset = 300;

	// x axis
	if (player->pos.x > camera.x + camera.w - xoffset) {
		float newRight;
		newRight = player->pos.x + xoffset;
		camera.x = newRight - camera.w;
	}
	else if (player->pos.x < camera.x + xoffset) {
		float newLeft;
		newLeft = player->pos.x - xoffset;
		camera.x = newLeft;
	}

	// y axis
	if (player->pos.y > camera.y + camera.h - yoffset) {
		// bottom
		float newBottom;
		newBottom = player->pos.y + yoffset;
		camera.y = newBottom - camera.h;
	}
	else if (player->pos.y < camera.y + yoffset) {
		// top
		float newTop;
		newTop = player->pos.y - yoffset;
		camera.y = newTop;
	}
}

// Param should be the position in the window the player clicked.
// This function will shoot it in worldspace (using camera). 
// Don't worry about camera shit when calling this.
void shootPlayerProjectile(int x, int y) {
	//printf("playerShootBullet called.");
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

	//printf("Shoot bullet at (%i,%i) in global worldspace.\n", xWorldPos, yWorldPos);
	//1
	float xVec, yVec;
	xVec = xWorldPos - player.pos.x;
	yVec = yWorldPos - player.pos.y;

	//2: normalize into unit vector
	float vectorMagnitude = sqrt((xVec * xVec) + (yVec * yVec));
	float xUnitVector = xVec / vectorMagnitude;
	float yUnitVector = yVec / vectorMagnitude;

	//3+4: this is the spawn point of the projectile.
	float circleEdgeX = player.pos.x + (xUnitVector * player.HITBOX_RADIUS);
	float circleEdgeY = player.pos.y + (yUnitVector * player.HITBOX_RADIUS);
	SDL_FPoint edgeSpawnPoint = { circleEdgeX, circleEdgeY };

	// Projectiles' velocity; just the unit vector multiplied by the projectile
	// speed.
	float xVel = xUnitVector * player.PROJECTILE_SPEED;
	float yVel = yUnitVector * player.PROJECTILE_SPEED;

	Projectile* newProj = new Projectile(edgeSpawnPoint, xVel, yVel, 10); //@refactor: put value in player object
	addEntity(newProj);
}

Entity* getEntityById(Uint16 id) {
	auto entry = entityIdMap.find(id);
	
	if (entry != entityIdMap.end()) {
		return entry->second;
	}
	return nullptr;
}

bool checkCollision(SDL_FRect* entity1, SDL_FRect* entity2) {
	/*SDL_FRect rect1 = *entity1->rect;
	SDL_FRect rect2 = *entity2->rect;*/
	SDL_FRect rect1 = *entity1; // TODO: change this function to match the params instead of this shit
	SDL_FRect rect2 = *entity2;

	bool xCollision = (((rect1.x + rect1.w) >= (rect2.x)) && ((rect2.x + rect2.w) >= (rect1.x)));
	bool yCollision = (((rect1.y + rect1.h) >= (rect2.y)) && ((rect2.y + rect2.h) >= (rect1.y)));

	return xCollision && yCollision;
}

int resolveEntityCollision(Entity* entity1, Entity* entity2) {
	switch (entity1->getEntityType()) {
	case PLAYER: {
			if (entity2->getEntityType() == ENEMY_PROJECTILE) {
				entity2->dead = true;
			}

			if (entity2->getEntityType() == ENEMY || entity2->getEntityType() == ENEMY_PROJECTILE) {
				// deal damage
				if (((Player*)entity1)->invulTime < SDL_GetTicks()) {
					printf("PLAYER HIT\n");
					entity1->hp--; // TODO: more advanced behavior when player dies/gets hit
					((Player*)entity1)->invulTime = SDL_GetTicks() + 1500;

					if (entity1->hp < 1) {
						gameOverTicks = SDL_GetTicks();
					}
				}
			}
		}
		break;
	case ENEMY:
		if (entity2->getEntityType() == PROJECTILE) {
			//removeEntity(entity1);
			//removeEntity(entity2);
			if (!entity2->dead) {
				//entity1->dead = true;
				entity2->dead = true;

				if (entity1->hp > 0) {
					entity1->hp--;
				}

				if (entity1->hp <= 0) {
					entity1->dead = true;
				}
			}
		}
		else if (entity2->getEntityType() == ENEMY) {
			// If they're different types of enemies, then ignore it entirely.
			if (((Enemy*)entity1)->getEnemyType() != ((Enemy*)entity2)->getEnemyType()) {
				return -4;
			}

			float xDistance, yDistance;
			if (entity1->getFRect()->x < entity2->getFRect()->x) {
				xDistance = ((entity1->getFRect()->x + entity1->getFRect()->w) - entity2->getFRect()->x);
			}
			else {
				xDistance = ((entity2->getFRect()->x + entity2->getFRect()->w) - entity1->getFRect()->x);
			}

			if (entity1->getFRect()->y < entity2->getFRect()->y) {
				yDistance = ((entity1->getFRect()->y + entity1->getFRect()->h) - entity2->getFRect()->y);
			}
			else {
				yDistance = ((entity2->getFRect()->y + entity2->getFRect()->h) - entity1->getFRect()->y);
			}

			if (xDistance > yDistance) {
				entity2->getFRect()->y -= yDistance;
			}
			else {
				entity2->getFRect()->x -= xDistance;
			}
		}
		break;
	case PROJECTILE:
		// call function for projectile behavior
		// will handle ENEMY_PROJECTILE (cancel each other out?)
		if (entity2->getEntityType() == ENEMY_PROJECTILE && !entity1->dead) {
			entity1->dead = true;

			if (!entity2->dead) {
				if (entity2->hp > 0) {
					entity2->hp--; // TODO: have projectile/entity hold 'damage' variable to deduct from this valuevvvvv
				}

				if (entity2->hp <= 0) {
					entity2->dead = true;
				}
			}
		}
		break;
	// Any after this has already been handled.
	}
	return 0;
}

// Removes this entity from the map, aswell as deleting the entity and setting nullptr.
void removeEntity(Entity* entity) {
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->id == entity->id) {
			entities.erase(entities.begin()+i);
			entityIdMap.erase(entity->id);
			qTree->remove(entity);
			delete entity;
			entity = nullptr;
			return;
		}
	}
}

void spawnEnemyTesting(int x, int y) {
	printf("spawnEnemyTesting called.");
	int xWorldPos = x + camera.x;
	int yWorldPos = y + camera.y;
	Enemy* newEnemy;
	// TODO: have functions for creating enemies inside the enemymanager so we don't have to worry about it in here.
	if (rand() % 2) {
		newEnemy = new SmallEnemy(xWorldPos, yWorldPos, enemySpawnLevel);
	}
	else {
		newEnemy = new MediumEnemy(xWorldPos, yWorldPos, enemySpawnLevel);
	}

	//newEnemy = new LargeEnemy(xWorldPos, yWorldPos, enemySpawnLevel);
	//newEnemy = new SmallEnemy(xWorldPos, yWorldPos, (rand() % 6)+1);
	//newEnemy = new SmallEnemy(xWorldPos, yWorldPos, enemySpawnLevel);

	entities.push_back(newEnemy);
	entityIdMap.insert({ newEnemy->id, newEnemy });

}

// Adds a new entity to the game state. (quadtree, entities vec, anything else)
void addEntity(Entity* entity) {
	entities.push_back(entity);
	entityIdMap.insert({ entity->id, entity });
}

// Loads the spawn file corresponding to the wave provided.
void loadSpawnFile(int wave) {
	int data = -1;
	Enemy* newEnemy;

	// Load in enemy spawn file
	std::ifstream spawnFile("assets/spawn" + std::to_string(wave) + ".info");
	if (spawnFile.is_open()) {
		while (spawnFile.good()) {
			// Safe to go through this file
			//for (int i = 0; i < HARDCODED_SPAWN_COUNT; i++) {
			spawnFile >> data;
			int type = data;
			spawnFile >> data;
			int level = data;
			spawnFile >> data;
			int xpos = data;
			spawnFile >> data;
			int ypos = data;
			spawnFile >> data;
			int spawntime = data;

			if (type == 0) {
				newEnemy = new SmallEnemy(xpos, ypos, level);
			}
			else if (type == 1) {
				newEnemy = new MediumEnemy(xpos, ypos, level);
			}
			else {
				newEnemy = new LargeEnemy(xpos, ypos, level);
			}
			newEnemy->spawnTime = spawntime;
			enemySpawnList.push(newEnemy);
			//}
		}
		printf("Loaded spawn%i.info\n", wave);
	}
	else {
		printf("Problem with loading spawn.info\n");
	}

	// Reset wave tick counters
	currentWaveTicks = 0;
	currentWaveStartTicks = SDL_GetTicks();
}

/* old code for resolving player vs enemy
* 
			Right now, this code is literally just teleporting the enemy to the closest position
			outside the player.

			What we'd rather happen is have the enemy be slowly pushed outside of the bounds of the
			player's hurtbox. This will be much cleaner and less jank.
			*/
			//float xDistance, yDistance;
			//bool xLeftSide, yTopSide;
			//if (entity1->getFRect()->x < entity2->getFRect()->x) {
			//	xDistance = ((entity1->getFRect()->x + entity1->getFRect()->w) - entity2->getFRect()->x);
			//	xLeftSide = true; // player is coming from left side
			//}
			//else {
			//	xDistance = ((entity2->getFRect()->x + entity2->getFRect()->w) - entity1->getFRect()->x);
			//	xLeftSide = false; // player is coming from right side
			//}

			//if (entity1->getFRect()->y < entity2->getFRect()->y) {
			//	yDistance = ((entity1->getFRect()->y + entity1->getFRect()->h) - entity2->getFRect()->y);
			//	yTopSide = true;
			//}
			//else {
			//	yDistance = ((entity2->getFRect()->y + entity2->getFRect()->h) - entity1->getFRect()->y);
			//	yTopSide = false;
			//}

			/*if (xDistance > yDistance) {
				entity2->getFRect()->y -= yDistance;
			}
			else {
				entity2->getFRect()->x -= xDistance;
			}*/

			// move the enemy a fixed distance from the player every frame if they're colliding.
			// if the x/y distance is lesx than this fixed distance, just move them far enough
			// that they are no longer colliding with the player.
			//if (!xLeftSide) {
			//	// enemy go to the right by fixed amount
			//	entity2->getFRect()->x += .01;
			//}
			//else {
			//	// go to the left by fixed amount
			//	entity2->getFRect()->x -= .01;
			//}