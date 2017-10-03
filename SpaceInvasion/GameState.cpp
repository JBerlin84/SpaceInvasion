#include "GameState.h"

GameState::GameState() {
	background = NULL;
}

bool GameState::isGameStateChanged() {
	return gameStateChanged;
}

void GameState::setNewGameState(state newGameState) {
	this->newGameState = newGameState;
	gameStateChanged = true;
}

state GameState::getNewGameState() {
	return newGameState;
}