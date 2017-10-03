#include "Enemy_21.h"

Enemy_21::Enemy_21(D3DXVECTOR2 position) : Enemy("21", position, 6, 350, 2) {
	setVelocity(D3DXVECTOR2(2, 0.4f));
}