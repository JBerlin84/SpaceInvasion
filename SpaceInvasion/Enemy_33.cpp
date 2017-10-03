#include "Enemy_33.h"

Enemy_33::Enemy_33(D3DXVECTOR2 position) : Enemy("33", position, 15, 0, 9) {
	float randomFloat = rand() / (float)RAND_MAX;
	float speedRange = maxSpeed - minSpeed;
	float xSpeed = randomFloat * speedRange + minSpeed;
	randomFloat = rand() / (float)RAND_MAX;
	float ySpeed = randomFloat * speedRange + minSpeed;
	setVelocity(D3DXVECTOR2(xSpeed, ySpeed));
}