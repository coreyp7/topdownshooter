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

	testbg = IMG_LoadTexture(renderer, "assets/bg2.jpg");
	if (testbg == NULL) {
		printf("bg not loaded %s", IMG_GetError());
	}

	enemyTexture = IMG_LoadTexture(renderer, "assets/enemy.png");
	if (enemyTexture == NULL) {
		printf("enemytexture not loaded %s", IMG_GetError());
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
	SDL_FRect bg = { 0, 0, 1000, 1500 }; //@hardcoded

	renderTextureRelativeToCamera(testbg, &bg);
	renderEnemies(gameState->getEnemies());
	renderProjectiles(gameState->getProjectiles());
	renderPlayer(gameState->getPlayer());

	drawQuadTree(gameState->getQuadTree());
}

///
///////////////// Helper functions. /////////////////
///

void Renderer::renderEnemies(std::vector<Enemy*> enemies) {
	renderedEnemiesThisFrame = 0;
	SDL_FRect* currRect;
	for (int i = 0; i < enemies.size(); i++) {
		currRect = enemies.at(i)->getFRect();

		bool xCollision = (((currRect->x + currRect->w) >= (camera.x)) && ((camera.x + camera.w) >= (currRect->x)));
		bool yCollision = (((currRect->y + currRect->h) >= (camera.y)) && ((camera.y + camera.h) >= (currRect->y)));

		if (xCollision && yCollision) {
			renderTextureRelativeToCamera(enemyTexture, currRect);
			renderedEnemiesThisFrame++;
		}
	}
}

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

void Renderer::renderPlayer(Player* player) {
	int radius = player->HITBOX_RADIUS;
	SDL_FPoint center = player->pos;
	SDL_FRect topLeft = { center.x - radius, center.y - radius, radius * 2, radius * 2 };
	this->renderTextureRelativeToCamera(playerTexture, &topLeft);

	raster_circle(center, radius);
}

void Renderer::renderProjectiles(std::vector<Projectile*> projectiles) {
	SDL_FRect* frect;
	for (int i = 0; i < projectiles.size(); i++) {
		frect = projectiles.at(i)->getFRect();
		renderTextureRelativeToCamera(playerTexture, frect);
	}
}

void Renderer::drawPointRelativeToCamera(int x, int y) {
	SDL_RenderDrawPointF(renderer, x - camera.x, y - camera.y);
}

void Renderer::drawRectRelativeToCamera(SDL_FRect rect) {
	rect.x -= camera.x;
	rect.y -= camera.y;
	SDL_RenderDrawRectF(renderer, &rect);
}

// Render this QuadTree and all of its children.
// Also draws all contained entities.
void Renderer::drawQuadTree(QuadTree* qTree) {
	SDL_FRect rect = { qTree->x, qTree->y, qTree->width, qTree->height };

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	drawRectRelativeToCamera(rect);

	if (qTree->isLeaf) {
		for (int i = 0; i < qTree->points.size(); i++) {
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			drawRectRelativeToCamera(*qTree->points[i]->getFRect());
		}
	}
	else {
		drawQuadTree(qTree->nw);
		drawQuadTree(qTree->ne);
		drawQuadTree(qTree->sw);
		drawQuadTree(qTree->se);
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