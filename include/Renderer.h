#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "Player.h"
#include "GameState.h"

class Renderer
{
public:
	Renderer(SDL_Window* window, SDL_Renderer* renderer);
	~Renderer();

	// Renders the entire GameState.
	// Everything starts here.
	void renderGameState(GameState* gameState);

	// Swaps back buffer to front and clears backbuffer (black).
	void showBackbufferClear();

	// Called to load assets from project for rendering.
	void setup();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Texture* playerTexture;
	SDL_Texture* enemyTexture;
	SDL_Texture* testbg;

	// Updated every frame from gamestate for rendering.
	// Used in like every function so keeping here for convenience.
	SDL_FRect camera;

	void renderPlayer(Player* player);

	void renderProjectiles(std::vector<Projectile*>* projectiles);

	void renderEnemies(std::vector<Enemy*>* enemies);

	void raster_circle(SDL_FPoint center, float radius);

	// "Helper" functions, which are really useful when creating new functions to
	// render specific things. These aren't called directly in renderGameState, 
	// but are extremely helpful for the functions that are. //

	// Makes rendering relative to camera alot easier. Textures only.
	void renderTextureRelativeToCamera(SDL_Texture* texture, SDL_FRect* dstrect);

	// Drawing debug stuff easily
	void drawPointRelativeToCamera(int x, int y);

	void drawRectRelativeToCamera(SDL_FRect rect);

	void drawQuadTree(QuadTree* qTree);

};

