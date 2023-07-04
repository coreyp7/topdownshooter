#include "Entity.h"

static Uint16 ID_COUNTER = 0;

Entity::Entity() {
	this->id = ID_COUNTER;
	ID_COUNTER++;
	this->dead = false;
}

Entity::~Entity() {

}