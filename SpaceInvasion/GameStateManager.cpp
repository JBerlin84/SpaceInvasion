#include "GameStateManager.h"

GameStateManager::GameStateManager() {
	currentState = menuState;
	player = new Player;
	loadingScreenIsDisplaying = false;

	gameStates.push_back(new MenuState);
	gameStates.push_back(new LevelStateBase(player, level1State));
	gameStates.push_back(new LevelStateBase(player, level2State));
	gameStates.push_back(new Level3State(player, level3State));
	gameStates.push_back(new LevelStateBase(player, level4State));
	gameStates.push_back(new Level5State(player, level5State));
	gameStates.push_back(new VictoryState());
	gameStates.push_back(new DefeatState());
	gameStates.push_back(new AboutState());
}

void GameStateManager::setState(state s) {
	currentState = s;
	gameStates[(int)currentState]->init();
}

void GameStateManager::draw() {
	if (gameStates[(int)currentState]->isGameStateChanged()){
		// Draw loading screen
		DrawLoading();
		loadingScreenIsDisplaying = true;
	}
	else {
		gameStates[(int)currentState]->draw();
	}
}

void GameStateManager::update() {
	// check if current state has changed, in that case, change it.
	// Make sure loading screen is displaying first before starting to load.
	if (gameStates[(int)currentState]->isGameStateChanged() && loadingScreenIsDisplaying) {
		// release old state, and initialize new state.
		gameStates[(int)currentState]->release();
		setState(gameStates[(int)currentState]->getNewGameState());
		loadingScreenIsDisplaying = false;
	}

	gameStates[(int)currentState]->update();
}