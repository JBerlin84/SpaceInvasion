#include "BossPart.h"

BossPart::BossPart(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR2 size, D3DXVECTOR2 position, int hitpoints, bool friendly, float velX, float velY, float angle, float rotation, int rateOfFire, int damage)
	: Entity(texture, size, position, hitpoints, friendly, velX, velY, angle, rotation) {
	this->rateOfFire = rateOfFire;
	this->damage = damage;
}

int BossPart::getDamage() {
	return damage;
}

bool BossPart::fire() {
	int rand = std::rand() % rateOfFire;
	if (rand == 0) {
		return true;
	}
	else {
		return false;
	}
}