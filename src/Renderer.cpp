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
    SDL_FPoint center = player->pos;
    int radius = player->HITBOX_RADIUS;
    center.x -= radius;
    center.y -= radius;
    SDL_FRect topLeft = {center.x, center.y, radius*2, radius*2};
    SDL_RenderCopyExF(renderer, playerTexture, NULL, 
        &topLeft, 0, NULL, SDL_FLIP_NONE);
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