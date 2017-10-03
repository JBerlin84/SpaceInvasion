#include "Enemy_20.h"

Enemy_20::Enemy_20(D3DXVECTOR2 position) : Enemy("20", position, 5, 350, 2) {
	setVelocity(D3DXVECTOR2(2, 0.4f));
}