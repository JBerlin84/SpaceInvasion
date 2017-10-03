/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
functions for the abstract class gameState
************************************************************************/

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "DirectX.h"
#include "Entity.h"
#include "GameStateEnum.h"

class GameState {
public:
	GameState();
	virtual bool init(int width = 3840, int height = 2160) = 0;
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void release() = 0;
	bool isGameStateChanged();
	void setNewGameState(state newGameState);
	state getNewGameState();
protected:
	LPDIRECT3DSURFACE9 background;
	bool gameStateChanged = false;	// If there is a new gamestate to go to, this is true;
	state newGameState;				// Sets the new gamestate to go to.
	int gameBoardWidth;
	int gameBoardHeight;
};

#endif