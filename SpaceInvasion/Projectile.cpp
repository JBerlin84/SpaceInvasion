#include "Projectile.h"

void Projectile::update() {
	xPos += velX;
	yPos += velY;
}

int Projectile::getDamage() {
	return hp;
}