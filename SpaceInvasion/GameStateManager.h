/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Gamestatemanager holds the gamestate of the game. Manages switches
between different levels.
Tells the game what to do.
************************************************************************/

#ifndef GAMESTATEMANAGER_H_
#define GAMESTATEMANAGER_H_

#include <vector>
#include "GameStateEnum.h"
#include "Player.h"
#include "DirectX.h"

#include "MenuState.h"
#include "LevelStateBase.h"
#include "Level3State.h"
#include "Level5State.h"
#include "AboutState.h"
#include "VictoryState.h"
#include "DefeatState.h"

using namespace std;	// for vector<T>

class GameStateManager {
public:
	GameStateManager();
	void setState(state);
	void draw();
	void update();
	void release();
	
private:
	state currentState;
	vector<GameState*> gameStates;
	Player *player;
	bool loadingScreenIsDisplaying;
};

#endif