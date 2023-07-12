#include "GameState.h"

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

// externs
Player player = Player(SDL_FPoint{ 250, 250 });
SDL_FRect camera = { 0, 0, 1280, 720 };
std::vector<Entity*> entities; // TODO: change to fixed sized array maybe
std::unordered_map<Uint16, Entity*> entityIdMap;
QuadTree* qTree = new QuadTree(0, 0, 1000, 1500);

// normal shit
float dt = 0;
Uint32 lastUpdate = 0;

void setupGameState() {
	entityIdMap.insert({ player.id, &player });
}

void simulateWorld() {
	// This approach to handling time is appropriate for this game.
	// Physics is simple and there's no networking.
	// If it becomes too inconsistent or gross later, then I will attempt
	// to refactor into example on 131/132 of 'Game Programming Patterns'.
	dt = (SDL_GetTicks() - lastUpdate) / 1000.f;
	lastUpdate = SDL_GetTicks();

	// Simulate entire game for dt seconds.
	player.simulate(dt);
	moveCameraWithPlayer(&player);

	// Update QuadTree (for now, deleting it and rebuilding it every frame).
	qTree->~QuadTree(); // @todo: have quadtree maintain itself (this is hard)
	qTree = new QuadTree(0, 0, 1000, 1500); // @hardcoded
	qTree->insert(&player);

	simulateEnemies();
	simulateProjectiles();

	resolveCollisions();

	// @lazy fix that adds extra O(n). delete stuff thats dead.
	// If I ever start having performance problems, I'm going
	// to change these to contiguous arrays.
	for (int i = 0; i < entities.size(); i++) {
		Entity* curr = entities[i];
		if (curr->dead) {
			entities.erase(entities.begin() + i);
			entityIdMap.erase(curr->id);
			qTree->remove(curr);
			delete curr;
			curr = nullptr;
			i--;

		}
	}
}

// Goes through every entity and finds if it collides with anything.
// When collisions occur, they will be resolved.
void resolveCollisions() {
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
	SDL_FRect qTreeRect = { 0, 0, 1000, 1500 }; // @hardcoded
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->getEntityType() == ENEMY) {
			Enemy* enemy = (Enemy*)entities[i];
			enemy->simulate(dt, player.getPosition());
			qTree->insert(entities[i]);
		}
	}
}

void simulateProjectiles() {
	SDL_FRect qTreeRect = { 0, 0, 1000, 1500 }; // @hardcoded
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
	case PLAYER:
		// call function for player behavior
		// will handle ENEMY, ENEMY_PROJECTILE
		if (entity2->getEntityType() == ENEMY) {
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
		else if (entity2->getEntityType() == ENEMY_PROJECTILE) {
			entity2->dead = true;
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
		//	// Prevent these two from colliding; fix their position to stay out of each other
		//	// @todo: move into function this is massive.
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

	// TODO: have functions for creating enemies inside the enemymanager so we don't have to worry about it in here.
	//Enemy* newEnemy = new SmallEnemy(xWorldPos, yWorldPos);
	Enemy* newEnemy = new MediumEnemy(xWorldPos, yWorldPos);
	entities.push_back(newEnemy);
	entityIdMap.insert({ newEnemy->id, newEnemy });

}

// Adds a new entity to the game state. (quadtree, entities vec, anything else)
void addEntity(Entity* entity) {
	entities.push_back(entity);
	entityIdMap.insert({ entity->id, entity });
}

