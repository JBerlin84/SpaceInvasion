#include "Win32.h"

using namespace std;

bool gameOver = false;

// Read config file
bool readConfig() {
	string line;
	ifstream config(configFile);
	if (config.is_open()) {
		getline(config, line);
		fullscreen = (line.compare("true") == 0 || line.compare("1") == 0) ? true : false;
		getline(config, line);
		screenWidth = strtol(line.c_str(), NULL, 10);
		getline(config, line);
		screenHeight = strtol(line.c_str(), NULL, 10);
		
		config.close();
	}
	else {
		return false;
	}
	
	return true;
}

// Windows event handling
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		gameOver = true;
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	if (!readConfig()) {
		return false;
	}
	scaling = (float)screenWidth / (float)gameBoardWidth;

	// Initialize window settings
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.lpszMenuName = NULL;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = "MainWindowClass";

	if (!RegisterClassEx(&wc)) {
		return false;
	}

	// Create window
	HWND hwnd;
	if (!fullscreen) {
		hwnd = CreateWindow(
			"MainWindowClass",
			TITLE.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			screenWidth, screenHeight,
			NULL,
			NULL,
			hInstance, 0);
	}
	else {
		hwnd = CreateWindow(
			"MainWindowClass",
			TITLE.c_str(),
			WS_EX_TOPMOST | WS_POPUP,
			0, 0,
			screenWidth, screenHeight,
			NULL,
			NULL,
			hInstance, 0);
	}
	if (hwnd == 0) {
		return 0;
	}

	// Display Window
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Initialize
	if (!Direct3D_Init(hwnd, &screenWidth, &screenHeight, fullscreen)) {return 0;}
	if (!DirectInput_Init(hwnd)) {return 0;}
	if (!DirectSound_Init(hwnd)) {return 0;}
	if (!GameInit(hwnd)) {return 0;}

	scaling = (float)screenWidth / (float)gameBoardWidth;	// Calculate correct scale based on returnvalue from Direct_3D Init

	// Main loop
	MSG msg;
	while (!gameOver) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		DirectInput_Update();

		GameTick(hwnd);
		GameDisplay(hwnd);
	}

	// Shut down
	GameEnd(hwnd);
	DirectSound_Shutdown();
	DirectInput_Shutdown();
	Direct3D_Shutdown();

	return msg.wParam;
}
