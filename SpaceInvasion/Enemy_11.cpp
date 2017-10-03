#include "Enemy_11.h"

Enemy_11::Enemy_11(D3DXVECTOR2 position) : Enemy("11", position, 2, 450, 1) {
	setVelocity(D3DXVECTOR2(3, 0.4f));
}