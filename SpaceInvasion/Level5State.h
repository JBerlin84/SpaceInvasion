/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Level 5 of the game, inherits levestatebase as this is a "special" level
************************************************************************/

#include "LevelStateBase.h"
#include "Boss.h"
#include "BossPart.h"

class Level5State : public LevelStateBase {
public:
	Level5State(Player *player, state thisGameState);
	bool init(int width = 3840, int height = 2160);
	void update();
	void draw();

protected:
	Boss *boss;
	void updateBoss();
	void updateBossParts();
	void drawBoss();
	bool isVictory();
	void checkCollisions();
	vector<BossPart*> bossParts;

	AnimationFrames *largeExplosion;
	AnimationFrames *smallExplosion;
};