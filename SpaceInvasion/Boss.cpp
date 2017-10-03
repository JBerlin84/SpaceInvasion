#include "Boss.h"

Boss::Boss(D3DXVECTOR2 position) : Entity() {

	hp = maxHp = 25;
	alive = true;
	friendly = false;

	shieldActive = false;
	hpBar.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Healthbar.png");

	stage1 = (float)maxHp;
	stage2 = maxHp * 0.75f;
	stage3 = maxHp * 0.5f;
	stage4 = maxHp * 0.25f;

	stage2Offset = { 0, 0 };
	stage3Offset = { 239, 59 };
	stage4Offset = { 0, 0 };

	isStage1 = true;
	isStage2, isStage3, isStage4 = false;
	bool hasSpawnedLeftWing = hasSpawnedRightWing = hasSpawnedSpiders = false;

	rateOfFireMain = 100;
	rateOfFireWing = 200;
	rateOfFireSpider = 350;
	mainWeaponDamage = 3;
	wingWeaponDamage = 7;
	spiderWeaponDamage = 2;

	spiderMoveTime = 5.5f;

	bossWholeNRMSpiders.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Boss_Whole_NRM_Spiders.png");
	bossWholeNRMSpiders.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Boss_Whole_NRM_Spiders.png");
	bossWholeShield.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Boss_Whole_SHIELD.png");
	bossWholeShield.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Boss_Whole_SHIELD.png");
	bossWholeNRM.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Boss_Whole_NRM.png");
	bossWholeNRM.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Boss_Whole_NRM.png");
	bossTwoPartsNRM.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Boss_2parts_NRM.png");
	bossTwoPartsNRM.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Boss_2parts_NRM.png");
	bossTwoPartsShield.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Boss_2parts_SHIELD.png");
	bossTwoPartsShield.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Boss_2parts_SHIELD.png");
	bossOnePartNRM.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Boss_1part_NRM.png");
	bossOnePartNRM.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Boss_1part_NRM.png");
	bossOnePartShield.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Boss_1part_SHIELD.png");
	bossOnePartShield.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Boss_1part_SHIELD.png");

	bossLeftWingNRM.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Left_Wing_NRM.png");
	bossLeftWingNRM.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Left_Wing_NRM.png");
	bossRightWingNRM.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Right_Wing_NRM.png");
	bossRightWingNRM.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Right_Wing_NRM.png");
	bossSpider.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Spider.png");
	bossSpider.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Spider.png");

	spiderWeapon.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Spider_weapon.png");
	spiderWeapon.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Spider_weapon.png");
	wingWeapon.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Missile.png");
	wingWeapon.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Missile.png");
	mainWeapon.texture = LoadTexture("Resources\\Enemies\\FinalBoss\\Main_weapon.png");
	mainWeapon.size = GetBitmapSize("Resources\\Enemies\\FinalBoss\\Main_weapon.png");

	setPosition(D3DXVECTOR2(position.x - bossWholeNRMSpiders.size.x/2, position.y));
}

void Boss::update() {
	Entity::update();
	width = (int)getTexture().size.x;
	height = (int)getTexture().size.y;

	if (hp <= stage2 && hp > stage3) {
		isStage2 = true;
		isStage1 = isStage3 = isStage4 = false;
		bspawnSpiders = true;
	}
	else if (hp <= stage3 && hp > stage4) {
		isStage3 = true;
		isStage1 = isStage2 = isStage4 = false;
		bspawnRightWing = true;
		if (!stage3Initialized) {
			xPos += stage3Offset.x;
			yPos += stage3Offset.y;
			stage3Initialized = true;
		}
	}
	if (hp <= stage4) {
		isStage4 = true;
		isStage1 = isStage2 = isStage3 = false;
		bspawnLeftWing = true;
	}

}

int Boss::getHp() {
	return hp;
}

Texture Boss::getTexture() {
	if (isStage1) {
		return bossWholeNRMSpiders;
	}
	else if (isStage2) {
		return immortal ? bossWholeShield : bossWholeNRM;
	}
	else if (isStage3) {
		return immortal ? bossTwoPartsShield : bossTwoPartsNRM;
	}
	else
		return immortal ? bossOnePartShield: bossOnePartNRM;
}

Texture Boss::getHealthBarTexture() {
	return hpBar;
}

Texture Boss::getLeftWingTexture() {
	return bossLeftWingNRM;
}
Texture Boss::getRightWingTexture() {
	return bossRightWingNRM;
}

Texture Boss::getSpiderTexture() {
	return bossSpider;
}

Texture Boss::getSpiderWeapon() {
	return spiderWeapon;
}

Texture Boss::getMainWeapon() {
	return mainWeapon;
}

Texture Boss::getWingWeapon() {
	return wingWeapon;
}

void Boss::setImmortality(bool immortality) {
	immortal = immortality;
}

bool Boss::isImmortal() {
	return immortal;
}

bool Boss::spawnLeftWing() {
	if (hasSpawnedLeftWing) {
		return false;
	}
	else {
		hasSpawnedLeftWing = bspawnLeftWing;
		return bspawnLeftWing;
	}
}

bool Boss::spawnRightWing() {
	if (hasSpawnedRightWing) {
		return false;
	}
	else {
		hasSpawnedRightWing = bspawnRightWing;
		return bspawnRightWing;
	}
}

bool Boss::spawnSpiders() {
	if (hasSpawnedSpiders) {
		return false;
	}
	else {
		spiderMoveTimer.restart();
		hasSpawnedSpiders = bspawnSpiders;
		return bspawnSpiders;
	}
}

void Boss::resetSpawnParts() {
	bspawnLeftWing = bspawnRightWing = bspawnSpiders = false;
}

bool Boss::playSpiderAnimation() {
	return isStage2 && spiderMoveTimer.getTime() < spiderMoveTime;
}

bool Boss::isAtStage2() {
	return isStage2;
}

bool Boss::isAtStage3() {
	return isStage3;
}

bool Boss::isAtStage4() {
	return isStage4;
}

bool Boss::fireRightWing() {
	int rand = std::rand() % rateOfFireWing;
	// The wing does not exist in these stages
	if (isStage3 || isStage4) {
		return false;
	}

	if (rand == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool Boss::fireLeftWing() {
	int rand = std::rand() % rateOfFireWing;
	// The wing does not exist in these stages
	if (isStage4) {
		return false;
	}

	if (rand == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool Boss::fireMainWeapon() {
	int rand = std::rand() % rateOfFireMain;
	if (rand == 0) {
		return true;
	}
	else {
		return false;
	}
}

int Boss::getMainWeaponDamage() {
	return mainWeaponDamage;
}

int Boss::getWingWeaponDamage() {
	return wingWeaponDamage;
}

int Boss::getSpiderWeaponDamage() {
	return spiderWeaponDamage;
}

int Boss::getSpiderWeaponRateOfFire() {
	return rateOfFireSpider;
}