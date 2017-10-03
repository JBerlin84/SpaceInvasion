/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Base enemy class. can be used to create a customized enemy
************************************************************************/

#ifndef ENEMY_H_
#define ENEMY_H_

#include "Entity.h"
#include "DirectX.h"

class Enemy : public Entity {
public:
	Enemy(const char * enemy, D3DXVECTOR2 position, int hp, int rateOfFire, int damage);
	void update();
	LPDIRECT3DTEXTURE9 getTexture();
	int getDamage();
	void hit(int damage);
	CSound* getWeaponSound();
	Texture getWeaponTexture();
	Texture getHealthBarTexture();
	bool fire();
protected:
	int value;							// How many points is the enemy worth?
	int rateOfFire;
	int damage;
	Texture healthBarTexture;
	Texture weaponTexture;
	CSound *weaponSound;
};

#endif