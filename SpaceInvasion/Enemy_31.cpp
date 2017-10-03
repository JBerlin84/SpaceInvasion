#include "Enemy_31.h"

Enemy_31::Enemy_31(D3DXVECTOR2 position) : Enemy("31", position, 2, 0, 2) {
	float randomFloat = rand() / (float)RAND_MAX;
	float speedRange = maxSpeed - minSpeed;
	float xSpeed = randomFloat * speedRange + minSpeed;
	randomFloat = rand() / (float)RAND_MAX;
	float ySpeed = randomFloat * speedRange + minSpeed;
	setVelocity(D3DXVECTOR2(xSpeed, ySpeed));
}