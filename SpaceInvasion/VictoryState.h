/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
A game state
Victory state, this is the state you get to when you beat the game
************************************************************************/

#ifndef VICTORYSTATE_H_
#define VICTORYSTATE_H_

#include "DirectX.h"
#include "GameState.h"
#include "GameStateEnum.h"

class VictoryState : public GameState {
public:
	VictoryState();
	bool init(int width, int height);
	void draw();
	void update();
	void release();
private:
	bool isButtonRepeat;
	bool pressedOnce;
	CSound *music;
	float letterPosition;
	float letterSpeed;
	int characterWidth;
};

#endif