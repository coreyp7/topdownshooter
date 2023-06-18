#include "InputManager.h"

InputManager::InputManager(Player* player) {
	this->player = player;
}

void InputManager::handleEvent(SDL_Event event) {
	// TODO: this'll have to change to something smarter to split
	// up this stuff into separate functions.
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		handleMovementEvent(event);
	}
	
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == 1) { // left click
			player->shootBullet(event.button.x, event.button.y);
		}
		printf("%i", event.button.button);
	}

}

void InputManager::handleMovementEvent(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
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
	else if (event.type == SDL_KEYUP) {
		switch (event.key.keysym.sym) {
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

