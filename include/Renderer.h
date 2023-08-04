#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "Player.h"
#include "GameState.h"

// TODO: move all the info stuff for debugging/imgui to its own module.
extern int renderedEnemiesThisFrame;

// Renders the entire GameState.
// Everything starts here.
void renderGameState(Player* player);

// Swaps back buffer to front and clears backbuffer (black).
void showBackbufferClear();

// Called to load assets from project for rendering.
void setupRenderer(SDL_Renderer* renderer, SDL_Window* window);

void renderPlayer(Player* player);

void renderProjectiles(std::vector<Projectile*> projectiles);

void renderEnemies(std::vector<Enemy*> enemies);

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

void renderEntityHealthBar(Entity* entity);

void renderExampleText(SDL_Texture* exampleText, int width, int height);

void renderGameOverText(SDL_Texture* gameOverText);