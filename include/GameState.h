#pragma once
#include <SDL.h>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <cmath>

#include "Player.h"
#include "Projectile.h"
#include "QuadTree.h"
#include "Enemy.h"
#include "EntityType.h"

#include "SmallEnemy.h"
#include "MediumEnemy.h"

#include "EnemyManager.h"

//class GameState {
//
//public:
//	GameState(Player* player);
//	~GameState();
extern Player player;
extern SDL_FRect camera;
extern std::vector<Entity*> entities;
extern std::unordered_map<Uint16, Entity*> entityIdMap;

// Simulate entire game state for dt seconds.
void simulateWorld();

Player* getPlayer();
std::vector<Projectile*> getProjectiles();
SDL_FRect getCamera();
QuadTree* getQuadTree();
std::vector<Enemy*> getEnemies();

// Shoots projectile from player to the specified point.
void playerShootBullet(int x, int y);

void spawnEnemyTesting(int, int); // for testing enemies

void shootEnemyProjectile(float x, float y, float xVel, float yVel, int size);

void addEntity(Entity* entity);

//private:

//Player* player;
//float dt;
//float lastUpdate;
/*std::vector<Projectile*> projectiles;
std::vector<Enemy*> enemies;*/
//std::vector<Entity*> entities;

//std::unordered_map<Uint16, Entity*> entityIdMap;

////EnemyManager* enemyManager;


//SDL_FRect camera;

//QuadTree* qTree;

// Updates camera position depending on player's position.
//void moveCameraWithPlayer();

// Also adds all of the projectiles to the QuadTree
void simulateProjectiles();

void simulateEnemies();

bool checkCollision(SDL_FRect* entity1, SDL_FRect* entity2);

void resolveCollisions();
int resolveEntityCollision(Entity* entity1, Entity* entity2);

Entity* getEntityById(Uint16 id);
void removeEntity(Entity* entity);
//};
