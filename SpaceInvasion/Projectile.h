/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
functions for the projectiles used in the game
************************************************************************/

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "Entity.h"
#include "Animation.h"

class Projectile :public Entity {
public:
	Projectile(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR2 size, D3DXVECTOR2 position, int damage, bool friendly = true, float velX = 0, float velY = 0, float rotation = 0)
		: Entity(texture, size, position, damage, friendly, velX, velY, rotation) {}
	void update();
	int getDamage();
};

#endif