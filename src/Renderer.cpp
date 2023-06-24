#include "Renderer.h"

Renderer::Renderer(SDL_Window* window, SDL_Renderer* renderer) {
	// Setup SDL & window/renderer
	this->window = window;
	this->renderer = renderer;
	//camera = NULL; // just incase there's an edge case where this is called
				   // when its not set yet
				   //
	//camera = {0, 0, 1280, 720}; //@hardcoded: this should be obtained from
								//params or something
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Renderer::setup() {
	playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
	if (playerTexture == NULL) {
		printf("Couldn't load player texture. %s", IMG_GetError());
	}

	testbg = IMG_LoadTexture(renderer, "assets/bg.jpg");
	if (testbg == NULL) {
		printf("bg not loaded %s", IMG_GetError());
	}
}

void Renderer::showBackbufferClear() {
	SDL_RenderPresent(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

// Renders entire game state. Utilizes functions for rendering
// specific things below this.
void Renderer::renderGameState(GameState* gameState) {
	//Setup camera for this frame, from gamestate
	this->camera = gameState->getCamera();
	SDL_FRect bg = { 0, 0, 3840, 2160 };

	renderTextureRelativeToCamera(testbg, &bg);
	renderProjectiles(gameState->getProjectiles());
	renderPlayer(gameState->getPlayer());
}

///
///////////////// Helper functions. /////////////////
///

// Used to easily render an SDL_Texture at its position relative to the camera.
// NOTE: this assumes that you don't need to rotate or anything like that.
void Renderer::renderTextureRelativeToCamera(
	SDL_Texture* texture,
	SDL_FRect* dstrect
) {
	SDL_FRect relativeToCamera = {
	  dstrect->x - camera.x, dstrect->y - camera.y, dstrect->w, dstrect->h
	};

	SDL_RenderCopyExF(renderer, texture, NULL, &relativeToCamera, 0, NULL, SDL_FLIP_NONE);
}

void Renderer::drawPointRelativeToCamera(int x, int y) {
	SDL_RenderDrawPointF(renderer, x - camera.x, y - camera.y);
}

void Renderer::renderPlayer(Player* player) {
	int radius = player->HITBOX_RADIUS;
	SDL_FPoint center = player->pos;
	SDL_FRect topLeft = { center.x - radius, center.y - radius, radius * 2, radius * 2 };
	//SDL_RenderCopyExF(renderer, playerTexture, NULL, &topLeft, 0, NULL, SDL_FLIP_NONE);
	this->renderTextureRelativeToCamera(playerTexture, &topLeft);

	raster_circle(center, radius);
}

void Renderer::renderProjectiles(
	std::vector<Projectile*>* projectiles) {
	SDL_FRect frect;
	for (int i = 0; i < projectiles->size(); i++) {
		frect = projectiles->at(i)->pos;
		//SDL_RenderCopyExF(renderer, playerTexture, NULL, &frect, 0, NULL, SDL_FLIP_NONE);
		renderTextureRelativeToCamera(playerTexture, &frect);
	}
}

//Obtained from https://rosettacode.org/wiki/Bitmap/Midpoint_circle_algorithm#C.
//Draws circle given point and radius.
//For debugging only (probably). @optimize: This could be rendered more efficiently but
//leaving for now.
void Renderer::raster_circle(SDL_FPoint center, float radius) //@move: could be moved to some kind of debug class/file or something.
{
	int x0 = center.x;
	int y0 = center.y;

	int f = 1 - radius;
	int ddF_x = 0;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	drawPointRelativeToCamera(x0, y0 + radius);
	drawPointRelativeToCamera(x0, y0 - radius);
	drawPointRelativeToCamera(x0 + radius, y0);
	drawPointRelativeToCamera(x0 - radius, y0);


	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;


		drawPointRelativeToCamera(x0 + x, y0 + y);
		drawPointRelativeToCamera(x0 - x, y0 + y);
		drawPointRelativeToCamera(x0 + x, y0 - y);
		drawPointRelativeToCamera(x0 - x, y0 - y);
		drawPointRelativeToCamera(x0 + y, y0 + x);
		drawPointRelativeToCamera(x0 - y, y0 + x);
		drawPointRelativeToCamera(x0 + y, y0 - x);
		drawPointRelativeToCamera(x0 - y, y0 - x);
	}
}
