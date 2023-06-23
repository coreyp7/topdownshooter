#include "Renderer.h"

Renderer::Renderer(SDL_Window* window, SDL_Renderer* renderer) {
    // Setup SDL & window/renderer
    this->window = window;
    this->renderer = renderer;
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
}

void Renderer::showBackbufferClear() {
    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

// Renders entire game state. Utilizes functions for rendering
// specific things below this.
void Renderer::renderGameState(GameState* gameState) {
    renderProjectiles(gameState->getProjectiles());
    renderPlayer(gameState->getPlayer());
}

// Helper functions.

void Renderer::renderPlayer(Player* player) {
    int radius = player->HITBOX_RADIUS;
    SDL_FPoint center = player->pos;
    SDL_FRect topLeft = { center.x - radius, center.y - radius, radius * 2, radius * 2 };
    SDL_RenderCopyExF(renderer, playerTexture, NULL, 
        &topLeft, 0, NULL, SDL_FLIP_NONE);
    raster_circle(center, radius);
}

void Renderer::renderProjectiles(
    std::vector<Projectile*>* projectiles) {
    SDL_FRect frect;
    for (int i = 0; i < projectiles->size(); i++) {
        frect = projectiles->at(i)->pos;
        SDL_RenderCopyExF(renderer, playerTexture, NULL,
            &frect, 0, NULL, SDL_FLIP_NONE);
    }
}

//Obtained from https://rosettacode.org/wiki/Bitmap/Midpoint_circle_algorithm#C.
//Draws circle given point and radius.
//For debugging only (probably).
//void Renderer::raster_circle(float x0, float y0, float radius)
void Renderer::raster_circle(SDL_FPoint center, float radius)
{   
    int x0 = center.x;
    int y0 = center.y;

    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_RenderDrawPointF(renderer, x0, y0 + radius);
    SDL_RenderDrawPointF(renderer, x0, y0 - radius);
    SDL_RenderDrawPointF(renderer, x0 + radius, y0);
    SDL_RenderDrawPointF(renderer, x0 - radius, y0);

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
        SDL_RenderDrawPointF(renderer, x0 + x, y0 + y);
        SDL_RenderDrawPointF(renderer, x0 - x, y0 + y);
        SDL_RenderDrawPointF(renderer, x0 + x, y0 - y);
        SDL_RenderDrawPointF(renderer, x0 - x, y0 - y);
        SDL_RenderDrawPointF(renderer, x0 + y, y0 + x);
        SDL_RenderDrawPointF(renderer, x0 - y, y0 + x);
        SDL_RenderDrawPointF(renderer, x0 + y, y0 - x);
        SDL_RenderDrawPointF(renderer, x0 - y, y0 - x);
    }
}