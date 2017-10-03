#include "Enemy_41.h"

Enemy_41::Enemy_41(D3DXVECTOR2 position) : Enemy("41", position, 13, 200, 4) {
	setVelocity(D3DXVECTOR2(2, 0.4f));
}