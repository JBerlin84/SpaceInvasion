#include "Enemy.h"

Enemy::Enemy(const char * enemy, D3DXVECTOR2 position, int hitpoints, int rateOfFire, int damage) : Entity() {

	//create base resource folder
	char resourcePath[50];
	strcpy_s(resourcePath, "Resources\\Enemies\\");
	strcat_s(resourcePath, enemy);
	
	// Create file paths
	char weaponSound[50];
	char enemyTexture[50];
	char weaponTexture[50];
	strcpy_s(weaponSound, resourcePath);
	strcat_s(weaponSound, ".wav");
	strcpy_s(enemyTexture, resourcePath);
	strcat_s(enemyTexture, ".png");
	strcpy_s(weaponTexture, resourcePath);
	strcat_s(weaponTexture, "_weapon.png");
	
	// Load data from filepaths
	texture = LoadTexture(enemyTexture);
	this->weaponTexture.texture = LoadTexture(weaponTexture);
	this->weaponTexture.size = GetBitmapSize(weaponTexture);
	healthBarTexture.texture = LoadTexture("Resources\\Enemies\\Healthbar.png");
	healthBarTexture.size = GetBitmapSize("Resources\\Enemies\\Healthbar.png");
	this->weaponSound = LoadSound(weaponSound);
	D3DXVECTOR2 sizeOfResource = GetBitmapSize(enemyTexture);
	width = (int)sizeOfResource.x;
	height = (int)sizeOfResource.y;

	maxHp = hp = hitpoints;
	friendly = false;
	this->rateOfFire = rateOfFire;
	this->damage = damage;

	setPosition(position);
}

void Enemy::update() {
	xPos += velX;
	yPos += velY;
}

LPDIRECT3DTEXTURE9 Enemy::getTexture() {
	return texture;
}

Texture Enemy::getWeaponTexture() {
	return weaponTexture;
}

Texture Enemy::getHealthBarTexture() {
	return healthBarTexture;
}

CSound* Enemy::getWeaponSound() {
	return weaponSound;
}

void Enemy::hit(int damage) {
	hp -= damage;

	if (hp <= 0) {
		kill();
	}
}

bool Enemy::fire() {
	int rand = std::rand() % rateOfFire;
	if (rand == 0) {
		return true;
	}
	else {
		return false;
	}
}

int Enemy::getDamage() {
	return damage;
}