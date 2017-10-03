#include "Enemy_40.h"

Enemy_40::Enemy_40(D3DXVECTOR2 position) : Enemy("40", position, 10, 200, 4) {
	setVelocity(D3DXVECTOR2(2, 0.4f));
}