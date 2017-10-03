/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Base game state. Most levels use this gamestate. Special levels inherits
this class.
************************************************************************/

#ifndef LEVELSTATEBASE_H_
#define LEVELSTATEBASE_H_

#include <fstream>

#include "DirectX.h"
#include "Player.h"
#include "Entity.h"
#include "GameStateEnum.h"
#include "GameState.h"
#include "Projectile.h"
#include "Enemy.h"

#include "Enemy_10.h"
#include "Enemy_11.h"
#include "Enemy_12.h"
#include "Enemy_20.h"
#include "Enemy_21.h"
#include "Enemy_30.h"
#include "Enemy_31.h"
#include "Enemy_32.h"
#include "Enemy_33.h"
#include "Enemy_34.h"
#include "Enemy_40.h"
#include "Enemy_41.h"

class LevelStateBase : public GameState {
public:
	LevelStateBase(Player *player, state thisGameState);
	bool init(int width = 3840, int height = 2160);
	void draw();
	void update();
	void release();

protected:
	bool init_enemyForce(char *filePath);
	int  isColliding(Entity *a, Entity *b);
	bool isVictory();
	bool isGameLost();
	void addProjectile(Texture texture, D3DXVECTOR2 position, D3DXVECTOR2 velocity, bool friendly = true, int damage = 1);
	void addAnimation(AnimationFrames* frames, D3DXVECTOR2 position);
	void updateVictoryPose();
	bool victoryPoseFinished;

	void updatePlayer();
	void updateEnemies();
	void updateDivEntities();
	void checkCollisions();

	void drawIntro();
	void drawPlayer();
	void drawEnemies();

	char resourceFolder[50];
	state thisGameState;

	Player *player;
	Enemy ***enemies;
	int numberOfEnemiesX;
	int numberOfEnemiesY;

	vector <Projectile*> projectiles;
	vector <Animation*> explosions;

	CSound *music;

	Texture text;
	Texture pressFireToStart;
	bool pressedFire;					// used to wait to start the game until player pressed fire once.

	Animation *intro;
	AnimationFrames *enemyExplosion;
};

#endif