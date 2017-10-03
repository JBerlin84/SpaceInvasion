#include "VictoryState.h"

VictoryState::VictoryState() {
	background = NULL;
}

bool VictoryState::init(int width, int height) {

	gameBoardWidth = width;
	gameBoardHeight = height;

	letterPosition = (float)(gameBoardHeight);
	letterSpeed = 1.2f;

	// "init" background
	background = LoadSurface("Resources\\Levels\\Victory\\Background.jpg");
	if (!background) {
		return false;
	}

	// init music
	music = LoadSound("Resources\\Levels\\Victory\\Music.Wav");
	LoopSound(music);

	gameStateChanged = false;	// Reset flag
	pressedOnce = false;		// Press enter twice to get out of the screen

	return true;
}

void VictoryState::update() {
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

void VictoryState::draw() {
	DrawBackground(background);

	const int numberOfElements = 50;

	// TODO: FIXA DETTA DJÄVULSKAPET
	string about[numberOfElements] = {
		"CONGRATULATIONS!!!",
		"",
		"",
		"THE INVASION HAS BEEN DESTROYED!",
		"",
		"YOU ALONE WAS STRONG ENOUGH TO TAKE",
		"DOWN THE ENEMY FORCE.",
		"",
		"I BET THEY REGRET",
		"TRYING TO INVADE",
		"THE EARTH!",
		"",
		"",
		"THANKS TO YOU,",
		"THE REST OF THE EARTHS POPULATION",
		"CAN LIVE IN PEACE!",
		"ALSO, I BET YOU WILL BE GREETED AS A",
		"TRUE HERO WHEN YOU GET BACK TO EARTH",
		"",
		"DON'T LET IT GET TO YOUR HEAD!!!",
		"THIS IS JUST A COMPUTER GAME!!!",
	};

	for (int i = 0; i < numberOfElements; i++) {
		string s = about[i];

		DrawString(400, (int)(letterPosition + 100 * i), s.c_str(), true);
	}
}

void VictoryState::release() {
	music->Stop();
}