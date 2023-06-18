#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <Player.h>

class Renderer
{
public:
	Renderer(SDL_Window* window, SDL_Renderer* renderer);
	~Renderer();

	void renderPlayer(Player* player);
	void renderPresent();
	void setup();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Texture* playerTexture;
};

