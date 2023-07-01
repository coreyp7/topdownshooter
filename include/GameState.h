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

class GameState {

public:
	GameState(Player* player);
	~GameState();

	// Simulate entire game state for dt seconds.
	void simulate();

	Player* getPlayer();
	std::vector<Projectile*>* getProjectiles();
	SDL_FRect getCamera();
	QuadTree* getQuadTree();
	std::vector<Enemy*>* getEnemies();

	// Shoots projectile from player to the specified point.
	void playerShootBullet(int x, int y);

private:

	Player* player;
	float dt;
	float lastUpdate;
	std::vector<Projectile*> projectiles;
	std::vector<Enemy*> enemies;

	std::unordered_map<Uint16, Entity*> entityIdMap;


	SDL_FRect camera;

	QuadTree* qTree;

	// Updates camera position depending on player's position.
	void moveCameraWithPlayer();

	// Also adds all of the projectiles to the QuadTree
	void simulateProjectiles();

	void simulateEnemies();

	bool checkCollision(SDL_FRect* entity1, SDL_FRect* entity2);

	void resolveCollisions();

	Entity* getEntityById(Uint16 id);
};
