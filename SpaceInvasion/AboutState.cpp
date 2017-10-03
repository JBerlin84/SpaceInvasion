#include "AboutState.h"

AboutState::AboutState() {
	background = NULL;
}

bool AboutState::init(int width, int height) {

	gameBoardWidth = width;
	gameBoardHeight = height;

	letterPosition = (float)(gameBoardHeight);
	letterSpeed = 1.2f;

	// "init" background
	background = LoadSurface("Resources\\Levels\\About\\Background.jpg");
	if (!background) {
		return false;
	}

	gameStateChanged = false;	// Reset flag
	pressedOnce = false;		// Press enter twice to get out of the screen

	return true;
}

void AboutState::update() {
	if (KeyDown(DIK_SPACE)) {
		if (!isButtonRepeat) {
			if (pressedOnce) {
				setNewGameState(menuState);	// new game
			}
			pressedOnce = true;
		}
		isButtonRepeat = true;
	}
	else {
		isButtonRepeat = false;
	}

	letterPosition -= letterSpeed;

	// Go back to menu state after text has passed.
	if (letterPosition < -(gameBoardHeight + 750)) {
		setNewGameState(menuState);
	}
}

void AboutState::draw() {
	DrawBackground(background);

	const int numberOfElements = 50;

	string about[numberOfElements] = {
		"SPACE INVASION",
		"",
		"",
		"A MODERN TAKE ON",
		"SPACE INVADERS",
		"",
		"",
		"THE GAME WAS CREATED AS AN ASSIGNMENT",
		"IN INTRODUCTION TO GAME PROGRAMMING ",
		"",
		"AN EXCITING COURSE TAKEN AT",
		"MID SWEDEN UNIVERSITY",
		"",
		"",
		"PROGRAMMING BY",
		"JIMMY BERLIN",
		"",
		"GRAPHICS BY",
		"CHRISTOPHER HOLGERSSON",
		"",
		"MUSIC AND SOUND BY",
		"ANDREAS ROHDIN",
		"",
		"",
		"MUSIC IN ORDER OF APPEARANCE",
		"LGTUNE",
		"ALONE IN THE ATTIC",
		"HURT IN HEART",
		"LENNART",
		"INFINITII",
		"DARK HEAVEN",
		"JUST NEW",
		"IONOSPHERE",
	};

	for (int i = 0; i < numberOfElements; i++) {
		string s = about[i];

		DrawString(400, (int)(letterPosition + 100 * i), s.c_str(), true);
	}
}

void AboutState::release() {
}