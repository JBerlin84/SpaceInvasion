/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Main boss in the game
************************************************************************/

#include "Enemy.h"
#include "DirectX.h"
#include "Timer.h"

class Boss : public Entity {
public:
	Boss(D3DXVECTOR2 position);
	void update();
	int getHp();
	bool isShieldActive();
	void setImmortality(bool immortality = true);
	bool isImmortal();
	bool isAtStage2();
	bool isAtStage3();
	bool isAtStage4();
	bool spawnLeftWing();
	bool spawnRightWing();
	bool spawnSpiders();
	void resetSpawnParts();
	bool playSpiderAnimation();

	Texture getTexture();
	Texture getHealthBarTexture();
	Texture getLeftWingTexture();
	Texture getRightWingTexture();
	Texture getSpiderTexture();
	Texture getSpiderWeapon();
	Texture getMainWeapon();
	Texture getWingWeapon();

	bool fireRightWing();
	bool fireLeftWing();
	bool fireMainWeapon();
	int getMainWeaponDamage();
	int getWingWeaponDamage();
	int getSpiderWeaponDamage();
	int getSpiderWeaponRateOfFire();
private:
	Texture bossWholeNRMSpiders;
	Texture bossWholeShield;
	Texture bossWholeNRM;
	Texture bossTwoPartsNRM;
	Texture bossTwoPartsShield;
	Texture bossOnePartNRM;
	Texture bossOnePartShield;
	Texture bossLeftWingNRM;
	Texture bossRightWingNRM;
	Texture bossSpider;
	Texture hpBar;
	Texture spiderWeapon;
	Texture wingWeapon;
	Texture mainWeapon;

	bool shieldActive;

	// Stage 1: spaceship flying around
	// Stage 2: spider robots detach
	// Stage 3: right wing detached
	// Stage 4: left wing detached
	float stage1, stage2, stage3, stage4;
	bool isStage1, isStage2, isStage3, isStage4;
	bool immortal;
	bool bspawnLeftWing, bspawnRightWing, bspawnSpiders;
	bool hasSpawnedLeftWing, hasSpawnedRightWing, hasSpawnedSpiders;
	bool stage2Initialized, stage3Initialized, stage4Initialized;
	D3DXVECTOR2 stage2Offset, stage3Offset, stage4Offset;	// used to offset the boss sprite so it wont move when parts fall off
	Timer spiderMoveTimer;	// Used to move the spider forward the correct time/length
	float spiderMoveTime;
	
	int rateOfFireWing;
	int rateOfFireMain;
	int rateOfFireSpider;
	int mainWeaponDamage;
	int wingWeaponDamage;
	int spiderWeaponDamage;
};