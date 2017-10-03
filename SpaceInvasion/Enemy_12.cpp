#include "Enemy_12.h"

Enemy_12::Enemy_12(D3DXVECTOR2 position) : Enemy("12", position, 3, 450, 1) {
	setVelocity(D3DXVECTOR2(3, 0.4f));
}