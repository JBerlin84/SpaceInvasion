/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
A game state
Menu state manages the main menu and the options menu
************************************************************************/

#ifndef MENUSTATE_H_
#define MENUSTATE_H_

#include "DirectX.h"
#include "GameState.h"
#include "GameStateEnum.h"

enum whichMenuState { main, options };

class MenuState : public GameState {
public:
	MenuState();
	bool init(int width, int height);
	void draw();
	void update();
	void release();
private:
	void drawMainMenu();
	void drawOptionsMenu();
	void updateMainMenu();
	void updateOptionsMenu();
	void saveConfig();

	int currentMenuChoice;
	whichMenuState currentMenuState;
	int optionsCurrentResolution;
	int optionsIsFullscreen;
	bool isButtonRepeat;
	bool isButtonRepeatAction;
	CSound *music;
	bool startMusic;
};

#endif