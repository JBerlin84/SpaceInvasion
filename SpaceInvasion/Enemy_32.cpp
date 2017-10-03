#include "Enemy_32.h"

Enemy_32::Enemy_32(D3DXVECTOR2 position) : Enemy("32", position, 3, 0, 2) {
	float randomFloat = rand() / (float)RAND_MAX;
	float speedRange = maxSpeed - minSpeed;
	float xSpeed = randomFloat * speedRange + minSpeed;
	randomFloat = rand() / (float)RAND_MAX;
	float ySpeed = randomFloat * speedRange + minSpeed;
	setVelocity(D3DXVECTOR2(xSpeed, ySpeed));
}