#include "MenuState.h"

MenuState::MenuState() {
	background = NULL;
	startMusic = true;
}

bool MenuState::init(int width, int height) {

	gameBoardWidth = width;
	gameBoardHeight = height;

	currentMenuChoice = 0;

	// "init" background
	background = LoadSurface("Resources\\Levels\\Menu\\Background.jpg");
	if (!background) {
		return false;
	}

	gameStateChanged = false;	// Reset flag

	if (startMusic) {
		music = LoadSound("Resources\\Levels\\Menu\\Music.wav");
		LoopSound(music);
	}

	currentMenuState = main;
	optionsCurrentResolution = 1;
	optionsIsFullscreen = fullscreen;

	return true;
}

void MenuState::update() {
	if (KeyDown(DIK_UP)) {
		if (!isButtonRepeat) {
			isButtonRepeat = true;
			--currentMenuChoice;
			if (currentMenuChoice < 0)
				currentMenuChoice = 0;
		}
	}
	else if (KeyDown(DIK_DOWN)) {
		if (!isButtonRepeat) {
			isButtonRepeat = true;
			++currentMenuChoice;
		}
	}
	else if (KeyDown(DIK_RETURN)) {
		if (!isButtonRepeat) {
			isButtonRepeat = true;
			if (currentMenuState == main) {
				updateMainMenu();
			}
			else if (currentMenuState == options) {
				updateOptionsMenu();
			}
		}
	}
	else {
		isButtonRepeat = false;
	}
}

void MenuState::updateMainMenu() {
	switch (currentMenuChoice % 4) {
	case 0:
		music->Stop();
		setNewGameState(level1State);		// new game
		break;
	case 1:
		startMusic = false;
		setNewGameState(aboutState);		// about
		break;
	case 2:
		currentMenuState = options;
		currentMenuChoice = 0;
		break;
	case 3:
		gameOver = true;
		break;
	}
}

void MenuState::updateOptionsMenu() {
	switch (currentMenuChoice % 4) {
	case 0:
		optionsCurrentResolution++;
		optionsCurrentResolution %= 4;
		break;
	case 1:
		optionsIsFullscreen = !optionsIsFullscreen;
		break;
	case 2:
		saveConfig();
		currentMenuState = main;
		currentMenuChoice = 0;
		break;
	case 3:
		currentMenuState = main;
		currentMenuChoice = 0;
		break;
	}
}

void MenuState::saveConfig() {

	string resolutions[] = { "1280x720", "1920x1080", "2560x1440", "3840x2160" };

	currentMenuChoice %= 4;
	string res = resolutions[optionsCurrentResolution];

	// Parse config
	// width
	size_t startPos = 0;
	size_t toCopy = res.find("x");
	string w = res.substr(startPos, toCopy);

	// height
	startPos = toCopy + 1;
	string h = res.substr(startPos);

	// store config
	ofstream config("Resources\\Config.cfg");
	if (config.is_open()) {
		config << (optionsIsFullscreen ? "true" : "false") << endl;
		config << w << endl;
		config << h << endl;
		config.close();
	}
}

void MenuState::draw() {
	DrawBackground(background);

	switch (currentMenuState)
	{
	case(main):
		drawMainMenu();
		break;
	case(options) :
		drawOptionsMenu();
		break;
	default:
		break;
	}
}

void MenuState::drawMainMenu() {

	DrawString(0, 500, "SPACE INVASION", true);

	string menuItems[] = { "NEW GAME", "ABOUT", "OPTIONS", "EXIT" };

	for (int i = 0; i < 4; i++) {
		if (i == currentMenuChoice % 4) {
			string s = "[ " + menuItems[i] + " ]";
			DrawString(40, (1600 + 100 * i), s.c_str(), true);
		}
		else {
			DrawString(40, (1600 + 100 * i), menuItems[i].c_str(), true);
		}
	}
}

void MenuState::drawOptionsMenu() {

	DrawString(845, 200, "OPTIONS", true);
	DrawString(845, 400, "NOTE - YOU HAVE TO RESTART THE GAME", true);
	DrawString(845, 500, "FOR THE CHANGES TO TAKE EFFECT", true);

	string menuItems[] = { "RESOLUTION", "FULLSCREEN", "SAVE", "CANCEL"};
	string resolutions[] = { "1280x720", "1920x1080", "2560x1440", "3840x2160" };

	for (int i = 0; i < 4; i++) {
		if (i == currentMenuChoice % 4) {
			string s = "[ " + menuItems[i] + " ]";
			DrawString(751, (1600 + 100 * i), s.c_str(), false);
		}
		else {
			DrawString(900, (1600 + 100 * i), menuItems[i].c_str(), false);
		}
	}


	DrawString(2200, 1600, resolutions[optionsCurrentResolution].c_str(), false);
	optionsIsFullscreen ? DrawString(2200, 1700, "(X)", false) : DrawString(2200, 1700, "( )", false);
}

void MenuState::release() {
	music->Stop();
}

