#include "InputManager.h"

//InputManager::InputManager(GameState* gameState) {
//	this->gameState = gameState;
//}
//extern Player* player;

void handleEvent(SDL_Event* event, Player* player) {
	// TODO: this'll have to change to something smarter to split
	// up this stuff into separate functions.
	if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
		handleMovementEvent(event, player);
	}

	if (event->type == SDL_MOUSEBUTTONDOWN) {
		if (event->button.button == 1) { // left click
			playerShootBullet(event->button.x, event->button.y);
		}
		else {
			spawnEnemyTesting(event->button.x, event->button.y);
		}
	}

}

//@TODO: call gameState function instead of calling a player
// function to move the player.
void handleMovementEvent(SDL_Event* event, Player* player) {
	if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
		case SDLK_w:
			player->startMoving(UP);
			break;
		case SDLK_s:
			player->startMoving(DOWN);
			break;
		case SDLK_a:
			player->startMoving(LEFT);
			break;
		case SDLK_d:
			player->startMoving(RIGHT);
			break;
		}
	}
	else if (event->type == SDL_KEYUP) {
		switch (event->key.keysym.sym) {
		case SDLK_w:
			player->stopMoving(UP);
			break;
		case SDLK_s:
			player->stopMoving(DOWN);
			break;
		case SDLK_a:
			player->stopMoving(LEFT);
			break;
		case SDLK_d:
			player->stopMoving(RIGHT);
			break;
		}
	}
}

