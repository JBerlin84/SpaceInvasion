/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Base class inherited by all the entities in the game, such as player,
enemy, boss, and so forth.
************************************************************************/

#ifndef ENTITY_H_
#define ENTITY_H_

#include "DirectX.h"

class Entity {
public:
	Entity(LPDIRECT3DTEXTURE9 texture = NULL, D3DXVECTOR2 size = { 0, 0 }, D3DXVECTOR2 position = { 0, 0 }, int hitpoints = 0, bool friendly = true, float velX = 0, float velY = 0, float angle = 0, float rotation = 0);
	LPDIRECT3DTEXTURE9 getTexture();
	D3DXVECTOR2 getPosition();
	D3DXVECTOR2 getSize();
	void setPosition(D3DXVECTOR2 position);
	void setVelocity(D3DXVECTOR2 velocity);
	D3DXVECTOR2 getVelocity();
	void moveEntity(D3DXVECTOR2 ammount);
	void changeDirection(bool x, bool y);
	void setDirection(int x, int y = 0);		//pass -1 for left or up, +1 for down or right
	void update();
	bool isAlive();
	void kill();
	bool isFriendly();
	void hit(int damage);
	float getAngle();
	void setRotation(float r);
	int getHealth();
	int getMaxHealth();

protected:
	LPDIRECT3DTEXTURE9 texture;
	int hp, maxHp;
	float xPos, yPos;
	int width, height;
	float velX, velY;
	bool friendly;
	bool alive;
	float angle, rotation;
};

#endif