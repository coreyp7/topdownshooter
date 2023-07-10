#include "Renderer.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* playerTexture;
SDL_Texture* sEnemyTexture;
SDL_Texture* mEnemyTexture;
SDL_Texture* testbg;
SDL_Texture* healthBarTexture;

int renderedEnemiesThisFrame = 0;

void setupRenderer(SDL_Renderer* newRenderer, SDL_Window* newWindow) {
	renderer = newRenderer;
	window = newWindow;

	playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
	if (playerTexture == NULL) {
		printf("Couldn't load player texture. %s", IMG_GetError());
	}

	testbg = IMG_LoadTexture(renderer, "assets/bg2.jpg");
	if (testbg == NULL) {
		printf("bg not loaded %s", IMG_GetError());
	}

	sEnemyTexture = IMG_LoadTexture(renderer, "assets/enemy.png");
	if (sEnemyTexture == NULL) {
		printf("sEnemyTexture not loaded %s", IMG_GetError());
	}

	mEnemyTexture = IMG_LoadTexture(renderer, "assets/matt.png");
	if (mEnemyTexture == NULL) {
		printf("mEnemyTexture not loaded %s", IMG_GetError());
	}

	healthBarTexture = IMG_LoadTexture(renderer, "assets/health.png");
	if (healthBarTexture == NULL) {
		printf("healthBarTexture not loaded %s", IMG_GetError());
	}

}

void showBackbufferClear() {
	SDL_RenderPresent(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

// Renders entire game state. Utilizes functions for rendering
// specific things below this.
void renderGameState(Player* player) {
	SDL_FRect bg = { 0, 0, 1000, 1500 }; //@hardcoded
	
	renderTextureRelativeToCamera(testbg, &bg); // test background to be replaced/improved later

	renderEnemies(getEnemies());
	renderProjectiles(getProjectiles());
	renderPlayer(player);

	drawQuadTree(qTree);
}

///
///////////////// Helper functions. /////////////////
///

void renderEnemies(std::vector<Enemy*> enemies) {
	renderedEnemiesThisFrame = 0;
	SDL_FRect* currRect;
	for (int i = 0; i < enemies.size(); i++) {
		currRect = enemies.at(i)->getFRect();

		// Only render if the enemy collides with the camera.
		bool xCollision = (((currRect->x + currRect->w) >= (camera.x)) && ((camera.x + camera.w) >= (currRect->x)));
		bool yCollision = (((currRect->y + currRect->h) >= (camera.y)) && ((camera.y + camera.h) >= (currRect->y)));

		if (xCollision && yCollision) {
			// @refactor: could change all entities to hold their texture, but this works fine.
			switch (enemies.at(i)->getEnemyType()) {
				case 0: renderTextureRelativeToCamera(sEnemyTexture, currRect); break;
				case 1: renderTextureRelativeToCamera(mEnemyTexture, currRect); break;
			}

			// draw enemy health bar
			SDL_FRect healthbar = { currRect->x, currRect->y + currRect->h, currRect->w, 5};
			SDL_FRect healthBarFill = healthbar;
			float fillMultiplier = healthBarFill.w / enemies.at(i)->maxHp;
			fillMultiplier *= enemies.at(i)->hp;
			healthBarFill.w = fillMultiplier;
			renderTextureRelativeToCamera(healthBarTexture, &healthBarFill);
			drawRectRelativeToCamera(healthbar);

			renderedEnemiesThisFrame++;
		}
	}
}

// Used to easily render an SDL_Texture at its position relative to the camera.
// NOTE: this assumes that you don't need to rotate or anything like that.
void renderTextureRelativeToCamera(
	SDL_Texture* texture,
	SDL_FRect* dstrect
) {
	SDL_FRect relativeToCamera = {
	  dstrect->x - camera.x, dstrect->y - camera.y, dstrect->w, dstrect->h
	};

	SDL_RenderCopyExF(renderer, texture, NULL, &relativeToCamera, 0, NULL, SDL_FLIP_NONE);
}

void renderPlayer(Player* player) {
	int radius = player->HITBOX_RADIUS;
	SDL_FPoint center = player->pos;
	SDL_FRect topLeft = { center.x - radius, center.y - radius, radius * 2, radius * 2 };
	renderTextureRelativeToCamera(playerTexture, &topLeft);

	raster_circle(center, radius);
}

void renderProjectiles(std::vector<Projectile*> projectiles) {
	SDL_FRect* frect;
	for (int i = 0; i < projectiles.size(); i++) {
		frect = projectiles.at(i)->getFRect();
		// Only render if the enemy collides with the camera.
		bool xCollision = (((frect->x + frect->w) >= (camera.x)) && ((camera.x + camera.w) >= (frect->x)));
		bool yCollision = (((frect->y + frect->h) >= (camera.y)) && ((camera.y + camera.h) >= (frect->y)));

		if (xCollision && yCollision) {
			renderTextureRelativeToCamera(playerTexture, frect);
		}
	}
}

void drawPointRelativeToCamera(int x, int y) {
	SDL_RenderDrawPointF(renderer, x - camera.x, y - camera.y);
}

void drawRectRelativeToCamera(SDL_FRect rect) {
	rect.x -= camera.x;
	rect.y -= camera.y;
	SDL_RenderDrawRectF(renderer, &rect);
}

// Render this QuadTree and all of its children.
// Also draws all contained entities.
void drawQuadTree(QuadTree* qTree) {
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
void raster_circle(SDL_FPoint center, float radius) //@move: could be moved to some kind of debug class/file or something.
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