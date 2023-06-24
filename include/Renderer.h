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
  SDL_Texture* testbg;

  // Updated every frame from gamestate for rendering.
  // Used in like every function so keeping here for convenience.
  SDL_FRect camera;

  // Makes rendering relative to camera alot easier. Textures only.
  void renderTextureRelativeToCamera(SDL_Texture* texture, SDL_FRect* dstrect);

  // Drawing debug stuff easily
  void drawPointRelativeToCamera(int x, int y);

  void renderPlayer(Player* player);

	void renderProjectiles(std::vector<Projectile*>* projectiles);

	void raster_circle(SDL_FPoint center, float radius);
};

