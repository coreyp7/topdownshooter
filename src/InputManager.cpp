#include "InputManager.h"

InputManager::InputManager(GameState* gameState) {
	this->gameState = gameState;
}

void InputManager::handleEvent(SDL_Event event) {
	// TODO: this'll have to change to something smarter to split
	// up this stuff into separate functions.
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		handleMovementEvent(event);
	}

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == 1) { // left click
			gameState->playerShootBullet(event.button.x, event.button.y);
		}
	}

}

//@TODO: call gameState function instead of calling a player
// function to move the player.
void InputManager::handleMovementEvent(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		case SDLK_w:
			gameState->getPlayer()->startMoving(UP);
			break;
		case SDLK_s:
			gameState->getPlayer()->startMoving(DOWN);
			break;
		case SDLK_a:
			gameState->getPlayer()->startMoving(LEFT);
			break;
		case SDLK_d:
			gameState->getPlayer()->startMoving(RIGHT);
			break;
		}
	}
	else if (event.type == SDL_KEYUP) {
		switch (event.key.keysym.sym) {
		case SDLK_w:
			gameState->getPlayer()->stopMoving(UP);
			break;
		case SDLK_s:
			gameState->getPlayer()->stopMoving(DOWN);
			break;
		case SDLK_a:
			gameState->getPlayer()->stopMoving(LEFT);
			break;
		case SDLK_d:
			gameState->getPlayer()->stopMoving(RIGHT);
			break;
		}
	}
}

