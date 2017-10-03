#include "Enemy_10.h"

Enemy_10::Enemy_10(D3DXVECTOR2 position) : Enemy("10", position, 1, 450, 1) {
	setVelocity(D3DXVECTOR2(3, 0.4f));
}