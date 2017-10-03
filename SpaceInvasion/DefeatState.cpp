#include "DefeatState.h"

DefeatState::DefeatState() {
	background = NULL;
}

bool DefeatState::init(int width, int height) {

	gameBoardWidth = width;
	gameBoardHeight = height;

	// "init" background
	background = LoadSurface("Resources\\Levels\\Defeat\\Background.jpg");
	if (!background) {
		return false;
	}

	// init music
	music = LoadSound("Resources\\Levels\\Defeat\\Music.Wav");
	LoopSound(music);

	// Init intro animation
	gameOverStart = new Animation("Resources\\Levels\\Defeat\\Animation\\", "GameOver_Start.txt", 60);
	gameOverLoop = new Animation("Resources\\Levels\\Defeat\\Animation\\", "GameOver_Loop.txt", 60, true);

	letterPosition = gameBoardHeight / 2 - gameOverStart->getSize().y / 2;
	letterSpeed = 1.5f;
	pauseTimer.restart();
	pauseTime = 4.0f;

	return true;
}

void DefeatState::update() {
	if (KeyDown(DIK_SPACE)) {
		setNewGameState(menuState);	// menu
	}

	if (pauseTimer.getTime() > pauseTime) {
		letterPosition -= letterSpeed;
	}
}

void DefeatState::draw() {
	DrawBackground(background);

	const int numberOfElements = 50;
	
	D3DXVECTOR2 position;
	position.x = gameBoardWidth / 2 - gameOverStart->getSize().x / 2;
	position.y = letterPosition;
	if (!gameOverStart->hasPlayedOnce()) {
		DrawSprite(gameOverStart->getFrame(), position, gameOverStart->getSize(), scaling);
	}
	else {
		DrawSprite(gameOverLoop->getFrame(), position, gameOverStart->getSize(), scaling);
	}

	// TODO: FIXA DETTA DJÄVULSKAPET
	string about[numberOfElements] = {
		"UNFORTUNATELY YOU WHERE NOT MAN ENOUGH",
		"TO DEFEND THE EARTH FROM THE INVADING",
		"ALIENS",
		"",
		"MAYBE YOU SHOULD NOT HAVE EATEN ALL THE",
		"CHIPS AND SODA YOU BOUGHT, BUT INSTEAD",
		"MADE SOME REAL TRAINING.",
		"",
		"JUST BECAUSE OF YOUR BAD CHARACTER",
		"ALL OF EARTH IS NOW SUFFERING!",
		"",
		"SHAME ON YOU!!!",
	};

	for (int i = 0; i < numberOfElements; i++) {
		string s = about[i];

		DrawString(0, (int)(letterPosition + 1500 + 120 * i), s.c_str(), true);
	}
}

void DefeatState::release() {
	music->Stop();
}

