/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Creates an Enemy-object with correct info for this enemy
************************************************************************/

#include "Enemy.h"

class Enemy_32 : public Enemy {
public:
	Enemy_32(D3DXVECTOR2 position);
private:
	float maxSpeed = 5;
	float minSpeed = -5;
};