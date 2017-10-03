#include "Entity.h"

Entity::Entity(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR2 size, D3DXVECTOR2 position, int hitpoints, bool friendly, float velX, float velY, float angle, float rotation) {
	this->texture = texture;
	width = (int)size.x;
	height = (int)size.y;
	xPos = position.x;
	yPos = position.y;
	maxHp = hp = hitpoints;
	this->friendly = friendly;
	this->velX = velX;
	this->velY = velY;
	this->angle = angle;
	this->rotation = rotation;
	alive = true;
}

LPDIRECT3DTEXTURE9 Entity::getTexture() {
	return texture;
}

D3DXVECTOR2 Entity::getPosition() {
	return D3DXVECTOR2(xPos, yPos);
}

D3DXVECTOR2 Entity::getSize() {
	return D3DXVECTOR2((float)width, (float)height);
}

void Entity::setPosition(D3DXVECTOR2 position) {
	xPos = position.x;
	yPos = position.y;
}

void Entity::setVelocity(D3DXVECTOR2 deltaPosition) {
	velX = deltaPosition.x;
	velY = deltaPosition.y;
}

D3DXVECTOR2 Entity::getVelocity() {
	return D3DXVECTOR2(velX, velY);
}

void Entity::moveEntity(D3DXVECTOR2 ammount) {
	xPos += ammount.x;
	yPos += ammount.y;
}

void Entity::changeDirection(bool x, bool y) {
	if (x)
		velX *= -1;
	if (y)
		velY *= -1;
}

void Entity::setDirection(int x, int y) {

	if (x) {
		if (!(x == (-1) || x == 1)) {
			return;
		}
		velX = abs(velX) * x;
	}
	if (y) {
		if (!(y == (-1) || y == 1)) {
			return;
		}
		velY = abs(velY) * y;
	}
}

void Entity::update() {
	angle += rotation;
	xPos += velX;
	yPos += velY;
}

bool Entity::isFriendly() {
	return friendly;
}

bool Entity::isAlive() {
	return alive;
}

void Entity::kill() {
	alive = false;
}

void Entity::hit(int damage) {
	--hp;
	if (hp <= 0) {
		kill();
	}
}

float Entity::getAngle() {
	return angle;
}

int Entity::getHealth() {
	return hp;
}

int Entity::getMaxHealth() {
	return maxHp;
}

void Entity::setRotation(float r) {
	rotation = r;
}