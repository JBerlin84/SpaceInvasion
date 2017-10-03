/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Holds the player in the game
************************************************************************/

#ifndef PLAYER_H_
#define PLAYER_H_

#include <chrono>

#include "Entity.h"
#include "DirectX.h"
#include "Animation.h"
#include "Timer.h"

class Player : public Entity {
public:
	Player();
	Player(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR2 size, D3DXVECTOR2 position, int hitpoints);
	void update();
	void goLeft();
	void goRight();
	void goUp();
	void noSteering();
	LPDIRECT3DTEXTURE9 getTexture();
	void setRateOfFire(float rate);
	float getRateOfFire();
	void addPoints(int pointsToAdd);
	int getPoints();
	Timer* getFireTimer();
	Timer* getShieldRechargeTimer();
	bool getDisplayShield();
	void hit(int damage);
	float getShield();
	float getMaxShield();
	CSound* getMissileSound();
	Texture getMissileTexture();
	Texture getShieldTexture();
	Texture getShieldBarTexture();
	Texture getHealthBarTexture();
	Texture getAfterburnerTexture();
	int getDamage();
	AnimationFrames* getMissileExplosion();
	AnimationFrames* getPlayerExplosion();

private:
	float    rateOfFire;
	float    acceleration, maxSpeed;
	int      points;
	int      damage;
	float    shield, maxShield;
	float    shieldDelayTime, shieldRegenSpeed, shieldDisplayTime;
	bool     displayShield;
	bool     steeringLeft, steeringRight;
	Texture  healthBarTexture;
	Texture  shieldBarTexture;
	Texture  missileTexture;
	CSound*  missileSound;
	Timer*   fireTimer;
	Timer*   shieldRechargeTimer;
	Timer*   shieldDisplayTimer;
	AnimationFrames* missileExplosionFrames;
	AnimationFrames* playerExplosionFrames;

	// Texture for player
	Texture left;
	Texture right;
	Texture straight;
	Texture leftShield;
	Texture rightShield;
	Texture straightShield;
	Texture afterburner;
};

#endif