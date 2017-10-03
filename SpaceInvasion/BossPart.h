/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
a specific boss part that releases during the boss fight will be of
this type.
************************************************************************/

#include "Entity.h"
#include "DirectX.h"
#include "Timer.h"

class BossPart : public Entity {
public:
	BossPart(LPDIRECT3DTEXTURE9 texture = NULL, D3DXVECTOR2 size = { 0, 0 }, D3DXVECTOR2 position = { 0, 0 }, int hitpoints = 0, bool friendly = true, float velX = 0, float velY = 0, float angle = 0, float rotation = 0, int rateOfFire = 500, int damage = 1);
	int getDamage();
	bool fire();
private:
	int rateOfFire;
	int damage;
};