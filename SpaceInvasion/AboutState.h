/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
about state, displays the about screen
************************************************************************/

#ifndef ABOUTSTATE_H_
#define ABOUTSTATE_H_

#include "DirectX.h"
#include "GameState.h"
#include "GameStateEnum.h"

class AboutState : public GameState {
public:
	AboutState();
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