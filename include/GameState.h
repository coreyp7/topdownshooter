#pragma once
#include <SDL.h>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <cmath>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>

#include "Player.h"
#include "Projectile.h"
#include "QuadTree.h"
#include "EntityType.h"

#include "Enemy.h"
#include "SmallEnemy.h"
#include "MediumEnemy.h"
#include "LargeEnemy.h"
#include "EnemyManager.h"

// All extern variables are used by the renderer.
extern Player player;
extern SDL_FRect camera;
extern std::vector<Entity*> entities;
extern std::unordered_map<Uint16, Entity*> entityIdMap;
extern QuadTree* qTree;
// imgui stuff
extern int enemySpawnLevel;

// Call this before calling any sort of 
// simulate function or else shit will break fast.
void setupGameState();

// Simulate entire game state (for delta time).
void simulateWorld();

// Getters which obtain vectors of specific entity types.
// Are required because all entities are stored in a single
// vector. An entity can be multiple things.
std::vector<Projectile*> getProjectiles();
std::vector<Enemy*> getEnemies();

// Shoots projectile from player to the specified point 
// (relative to window position; is translated to world space).
void shootPlayerProjectile(int x, int y);

// TODO: replace with something more general for shooting projectiles.
// is called by the enemymanager when you wanna shoot a projectile.
void shootEnemyProjectile(float x, float y, float xVel, float yVel, int size);

// For adding an entity to the entire system.
// TODO: move entity access/manipulation into entitymanager file or something.
void addEntity(Entity* entity);

// Testing stuff go down here.
void spawnEnemyTesting(int, int);

void loadSpawnFile(int wave);
