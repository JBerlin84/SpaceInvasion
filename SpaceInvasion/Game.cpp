#include "DirectX.h"
#include "GameStateManager.h"

const extern string TITLE = "Space Invasion";

GameStateManager *gsm;
int gameBoardWidth = 3840;
int gameBoardHeight = 2160;

// Game initialization
bool GameInit(HWND hwnd) {

	// Init gamestatemanager
	gsm = new GameStateManager();
	gsm->setState(menuState);

	srand((unsigned int)time(NULL));

	return true;
}


// Game update function
void GameTick(HWND hwnd) {

	gsm->update();

	// exit if escape
	if (KeyDown(DIK_ESCAPE)) {
		gameOver = true;
	}
}

// Game draw function
void GameDisplay(HWND hwnd) {
	if (!d3ddev) {
		return;
	}
	// Clear the back buffer.
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(200, 0, 100), 1.0f, 0);
	// start render
	if (d3ddev->BeginScene()) {

		// Start sprite object
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		gsm->draw();

		spriteobj->End();
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
}

void GameEnd(HWND hwnd) {
}