/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Level 3 of the game, inherits levestatebase as this is a "special" level
************************************************************************/

#include "LevelStateBase.h"

class Level3State : public LevelStateBase {
public:
	Level3State(Player *player, state thisGameState);
	void update();
	bool init(int width = 3840, int height = 2160);

protected:
	bool isGameLost();
	void updateEnemies();
	void checkCollisions();
	Timer hitTimer;
	float invulnerabilityTime;				// used to protect player in extreme situations when a lot of comets come at the same time.
	AnimationFrames *cometExplosionFrames;
};