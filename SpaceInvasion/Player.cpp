#include "Player.h"

Player::Player() : Entity() {
	left.texture = LoadTexture("Resources\\Player\\Player_Left.png");
	right.texture = LoadTexture("Resources\\Player\\Player_Right.png");
	straight.texture = LoadTexture("Resources\\Player\\Player_Straight.png");
	leftShield.texture = LoadTexture("Resources\\Player\\Player_Shield_Left.png");
	rightShield.texture = LoadTexture("Resources\\Player\\Player_Shield_Right.png");
	straightShield.texture = LoadTexture("Resources\\Player\\Player_Shield_Straight.png");
	// Store size in all texture structures
	left.size = right.size = straight.size = leftShield.size = rightShield.size = straightShield.size = GetBitmapSize("Resources\\Player\\Player_Straight.png");
	
	afterburner.texture = LoadTexture("Resources\\Player\\Afterburner.png");
	afterburner.size = GetBitmapSize("Resources\\Player\\Afterburner.png");

	missileTexture.texture = LoadTexture("Resources\\Player\\Missile.png");
	missileTexture.size = GetBitmapSize("Resources\\Player\\Missile.png");
	shieldBarTexture.texture = LoadTexture("Resources\\Player\\Shieldbar.png");
	shieldBarTexture.size = GetBitmapSize("Resources\\Player\\Shieldbar.png");
	healthBarTexture.texture = LoadTexture("Resources\\Player\\Healthbar.png");
	healthBarTexture.size = GetBitmapSize("Resources\\Player\\Healthbar.png");
	missileSound = LoadSound("Resources\\Player\\Missile.wav");
	
	this->setRateOfFire(rateOfFire);
	this->texture = texture;
	width = (int)straight.size.x;
	height = (int)straight.size.y;
	maxHp = hp = 10;
	maxShield = shield = 3;
	rateOfFire = 0.4f;
	friendly = true;
	shieldDelayTime = 2.0f;
	shieldRegenSpeed = 0.005f;
	shieldDisplayTime = 0.3f;
	maxSpeed = 7;
	acceleration = 0.3f;
	steeringLeft = steeringRight = false;
	damage = 1;

	fireTimer = new Timer();
	shieldRechargeTimer = new Timer();
	shieldDisplayTimer = new Timer();
	fireTimer->restart();
	shieldRechargeTimer->restart();
	shieldDisplayTimer->restart();
	
	missileExplosionFrames = new AnimationFrames("Resources\\Player\\MissileExplosion\\", "Explosion.txt");
	playerExplosionFrames = new AnimationFrames("Resources\\Player\\PlayerExplosion\\", "Explosion.txt");
}

Player::Player(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR2 size, D3DXVECTOR2 position, int hitpoints)
	: Entity(texture, size, position, hitpoints, true, 0, 0) {
}

void Player::update() {
	xPos += velX;
	yPos += velY;

	if (shieldRechargeTimer->getTime() > shieldDelayTime && shield < maxShield) {
		shield += shieldRegenSpeed;
	}
	if (shieldDisplayTimer->getTime() > shieldDisplayTime) {
		displayShield = false;
	}
}

void Player::goUp() {
	velY -= acceleration * 0.2f;
}

void Player::goLeft() {
	velX -= acceleration;
	if (abs(velX) > maxSpeed) {
		velX = -maxSpeed;
	}
	steeringLeft = true;
	steeringRight = false;
}

void Player::goRight() {
	velX += acceleration;
	if (abs(velX) > maxSpeed) {
		velX = maxSpeed;
	}
	steeringRight = true;
	steeringLeft = false;
}

void Player::noSteering() {
	if (abs(velX) > 0.5) {
		if (velX > 0)
			velX -= acceleration;
		if (velX < 0)
			velX += acceleration;
	}
	else {
		velX = 0;
	}
	steeringLeft = false;
	steeringRight = false;
}

void Player::setRateOfFire(float rate) {
	rateOfFire = rate;
}

float Player::getRateOfFire() {
	return rateOfFire;
}

LPDIRECT3DTEXTURE9 Player::getTexture() {
	if (steeringLeft) return left.texture;
	if (steeringRight) return right.texture;
	return straight.texture;
}

Texture Player::getMissileTexture() {
	return missileTexture;
}

AnimationFrames* Player::getMissileExplosion() {
	return missileExplosionFrames;
}

AnimationFrames* Player::getPlayerExplosion() {
	return playerExplosionFrames;
}

Texture Player::getShieldTexture() {
	if (steeringLeft) return leftShield;
	if (steeringRight) return rightShield;
	return straightShield;
}

Texture Player::getShieldBarTexture() {
	return shieldBarTexture;
}

Texture Player::getHealthBarTexture() {
	return healthBarTexture;
}

Texture Player::getAfterburnerTexture() {
	return afterburner;
}

CSound* Player::getMissileSound() {
	return missileSound;
}

void Player::addPoints(int pointsToAdd) {
	points += pointsToAdd;
}

int Player::getPoints() {
	return points;
}

float Player::getShield() {
	return shield;
}

float Player::getMaxShield() {
	return maxShield;
}

Timer* Player::getFireTimer() {
	return fireTimer;
}

Timer* Player::getShieldRechargeTimer() {
	return shieldRechargeTimer;
}

void Player::hit(int damage) {

	float remainingShield = shield;
	if (shield > 0) {
		shield -= damage;
		damage = damage - (int)remainingShield;
		displayShield = true;
		shieldDisplayTimer->restart();
	}
	if (damage > 0) {
		hp -= damage;
	}
	if (shield < 0) {
		shield = 0;
	}

	shieldRechargeTimer->restart();

	if (hp <= 0) {
			kill();
	}
}

bool Player::getDisplayShield() {
	return displayShield;
}

int Player::getDamage() {
	return damage;
}