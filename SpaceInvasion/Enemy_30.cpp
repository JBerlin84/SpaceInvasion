#include "Enemy_30.h"

Enemy_30::Enemy_30(D3DXVECTOR2 position) : Enemy("30", position, 1, 0, 2) {
	float randomFloat = rand() / (float)RAND_MAX;
	float speedRange = maxSpeed - minSpeed;
	float xSpeed = randomFloat * speedRange + minSpeed;
	randomFloat = rand() / (float)RAND_MAX;
	float ySpeed = randomFloat * speedRange + minSpeed;
	setVelocity(D3DXVECTOR2(xSpeed, ySpeed));
}