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

	void renderGameState(GameState* gameState);

	// Swaps back buffer to front and clears backbuffer (black).
	void showBackbufferClear();

	// Called to load assets from project for rendering.
	void setup();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Texture* playerTexture;

	void renderPlayer(Player* player);

	void renderProjectiles(std::vector<Projectile*>* projectiles);
};

