#pragma once
#include <SDL.h>
class Projectile
{
public:
	// Create projectile at position, where position is the center of the
	// projectile.
	Projectile(SDL_FPoint pos, float xVel, float yVel);

	~Projectile();

	void simulate(float dt);

	SDL_FRect pos;

	float xVel, yVel;

	const float SIZE = 8;
};

