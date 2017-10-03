/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Defeat state, this is the game over screen
************************************************************************/

#ifndef DEFEATSTATE_H_
#define DEFEATSTATE_H_

#include "DirectX.h"
#include "GameState.h"
#include "GameStateEnum.h"
#include "Animation.h"

class DefeatState : public GameState {
public:
	DefeatState();
	bool init(int width, int height);
	void draw();
	void update();
	void release();
private:
	CSound *music;

	Animation *gameOverStart;
	Animation *gameOverLoop;
	float letterPosition;
	float letterSpeed;
	Timer pauseTimer;
	float pauseTime;
};

#endif