#include "Enemy_34.h"

Enemy_34::Enemy_34(D3DXVECTOR2 position) : Enemy("34", position, 15, 0, 9) {
	float randomFloat = rand() / (float)RAND_MAX;
	float speedRange = maxSpeed - minSpeed;
	float xSpeed = randomFloat * speedRange + minSpeed;
	randomFloat = rand() / (float)RAND_MAX;
	float ySpeed = randomFloat * speedRange + minSpeed;
	setVelocity(D3DXVECTOR2(xSpeed, ySpeed));
}