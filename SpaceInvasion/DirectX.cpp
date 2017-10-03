#include <iostream>
#include "DirectX.h"

using namespace std;

// Direct3D Variables
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
LPD3DXSPRITE spriteobj = NULL;
LPDIRECT3DTEXTURE9 fontTexture = NULL;
FontStructure **fontStructure = NULL;
int numberOfFontChars = 0;
LPDIRECT3DSURFACE9 loadingScreen = NULL;

// DirectInput variables
LPDIRECTINPUT8 dinput = NULL;
LPDIRECTINPUTDEVICE8 mouse = NULL;
LPDIRECTINPUTDEVICE8 keyboard = NULL;
DIMOUSESTATE mouseState;
char keys[256];

// DirectSound variables
CSoundManager *dSound = NULL;

bool Direct3D_Init(HWND hwnd, int *width, int *height, bool fullscreen) {
	// Init Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL) {
		return false;
	}

	// Set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	if (!fullscreen) {
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferWidth = *width;
		d3dpp.BackBufferHeight = *height;
		d3dpp.hDeviceWindow = hwnd;
	}
	else {
		D3DDISPLAYMODE dm;
		d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);

		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = FALSE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = dm.Format;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferWidth = *width = dm.Width;
		d3dpp.BackBufferHeight = *height = dm.Height;
		d3dpp.hDeviceWindow = hwnd;
	}

	// create Direct3D device
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	if (d3ddev == NULL) {
		return false;
	}

	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	D3DXCreateSprite(d3ddev, &spriteobj);

	loadingScreen = LoadSurface("Resources\\Loading.jpg");
	if (!loadingScreen) {
		return false;
	}

	LoadFont();

	return true;
}

void Direct3D_Shutdown() {
	fontTexture->Release();

	if (spriteobj) {
		spriteobj->Release();
		spriteobj = NULL;
	}
	if (d3ddev) {
		d3ddev->Release();
		d3ddev = NULL;
	}
	if (d3d) {
		d3d->Release();
		d3d = NULL;
	}
}

bool DirectInput_Init(HWND hwnd) {
	// Initialize DInput object
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL);

	// Initialize Keyboard
	dinput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	keyboard->Acquire();

	// Initialize Mouse
	dinput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	mouse->SetDataFormat(&c_dfDIMouse);
	mouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	mouse->Acquire();
	d3ddev->ShowCursor(false);

	return true;
}

void DirectInput_Shutdown() {
	if (keyboard) {
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = NULL;
	}
	if (mouse) {
		mouse->Unacquire();
		mouse->Release();
		mouse = NULL;
	}
}

void DirectInput_Update() {
	mouse->GetDeviceState(sizeof(mouseState), (LPVOID)&mouseState);
	keyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);
}

int KeyDown(int key) {
	return (keys[key] & 0x80);
}

int MouseButton(int button) {
	return (mouseState.rgbButtons[button] & 0x80);
}

int MouseX() {
	return mouseState.lX;
}

int MouseY() {
	return mouseState.lY;
}

bool DirectSound_Init(HWND hwnd) {
	dSound = new CSoundManager();

	HRESULT result;
	result = dSound->Initialize(hwnd, DSSCL_PRIORITY);
	if (result != DS_OK) {
		return false;
	}

	result = dSound->SetPrimaryBufferFormat(2, 22050, 16);
	if (result != DS_OK) {
		return false;
	}

	return true;
}

void DirectSound_Shutdown() {
	if (dSound) {
		delete dSound;
	}
}

CSound *LoadSound(string filename) {
	HRESULT result;
	CSound *wave = NULL;

	char s[255];
	sprintf_s(s, "%s", filename.c_str());
	result = dSound->Create(&wave, s);
	if (result != DS_OK) {
		wave = NULL;
	}
	return wave;
}

void PlaySound(CSound *sound) {
	sound->Play();
}

void LoopSound(CSound *sound) {
	sound->Play(0, DSBPLAY_LOOPING);
}

void StopSound(CSound *sound) {
	sound->Stop();
}

void DrawSurface(LPDIRECT3DSURFACE9 dest, float xPos, float yPos, LPDIRECT3DSURFACE9 source) {
	// Fetch description
	D3DSURFACE_DESC desc;
	source->GetDesc(&desc);

	// Create surface
	RECT sourceRect = { 0, 0, desc.Width, desc.Height };
	RECT destRect = { (long)xPos, (long)yPos, (long)xPos+desc.Width, (long)yPos+desc.Height };

	// Draw surface
	d3ddev->StretchRect(source, &sourceRect, dest, &destRect, D3DTEXF_NONE);
}

void DrawBackground(LPDIRECT3DSURFACE9 source) {
	d3ddev->StretchRect(source, NULL, backbuffer, NULL, D3DTEXF_NONE);
}

void DrawLoading() {
	DrawBackground(loadingScreen);
	DrawString(0, 300, "LOADING", true);
}

LPDIRECT3DSURFACE9 LoadSurface(string filename) {
	LPDIRECT3DSURFACE9 image = NULL;

	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
	if (result != D3D_OK) {
		return NULL;
	}

	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width,
		info.Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&image,
		NULL);
	if (result != D3D_OK) {
		return NULL;
	}

	result = D3DXLoadSurfaceFromFile(
		image,
		NULL,
		NULL,
		filename.c_str(),
		NULL,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 0, 0),
		NULL);
	if (result != D3D_OK) {
		return NULL;
	}

	return image;
}

void DrawTexture(float xPos, float yPos, LPDIRECT3DTEXTURE9 texture) {
	D3DXVECTOR3 position(xPos, yPos, 0);
	spriteobj->Draw(texture, NULL, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
}

D3DXVECTOR2 GetBitmapSize(string filename) {
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);

	if (result == D3D_OK)
		return D3DXVECTOR2((float)info.Width, (float)info.Height);
	else
		return D3DXVECTOR2(0, 0);
}

LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor) {
	LPDIRECT3DTEXTURE9 texture = NULL;
	
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);

	if (result != D3D_OK)
		return NULL;

	D3DXCreateTextureFromFileEx(
		d3ddev,
		filename.c_str(),
		info.Width, info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transcolor,
		&info,
		NULL,
		&texture);

	if (result == D3D_OK)
		return texture;
	else
		return NULL;
}

// Draw image from spritesheet
void DrawSprite(LPDIRECT3DTEXTURE9 sprite, D3DXVECTOR2 destination, D3DXVECTOR2 size, float scaling, float rotation,
	int frameNumber, int columns, D3DCOLOR color) {

	D3DXVECTOR2 scale(scaling, scaling);
	int x = (int)destination.x;
	int y = (int)destination.y;
	D3DXVECTOR2 translation(x*scaling, y*scaling);
	D3DXVECTOR2 center(size.x*scaling / 2, size.y*scaling / 2);

	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, &center, rotation, &translation);

	spriteobj->SetTransform(&matrix);

	RECT sourceRect;
	sourceRect.left = (long)((frameNumber % columns) * size.x);
	sourceRect.top = (long)((frameNumber / columns) * size.y);
	sourceRect.right = (long)(sourceRect.left + size.x);
	sourceRect.bottom = (long)(sourceRect.top + size.y);

	spriteobj->Draw(sprite, &sourceRect, NULL, NULL, color);
}

// Draw image from spritesheet
void DrawChar(D3DXVECTOR2 destination, int sourceX, int sourceY, int width, int height, float scaling) {

	D3DXVECTOR2 scale(scaling, scaling);
	int x = (int)destination.x;
	int y = (int)destination.y;
	D3DXVECTOR2 translation(x*scaling, y*scaling);
	D3DXVECTOR2 center((float)width*scaling / 2, (float)height*scaling / 2);

	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(&matrix, NULL, 0, &scale, &center, 0, &translation);

	spriteobj->SetTransform(&matrix);

	RECT sourceRect;
	sourceRect.left = sourceX;
	sourceRect.top = sourceY;
	sourceRect.right = sourceX + width;
	sourceRect.bottom = sourceY + height;

	spriteobj->Draw(fontTexture, &sourceRect, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
}

bool LoadFont() {
	fontTexture = LoadTexture("Resources\\font.png");
	if (!fontTexture) {
		return false;
	}

	string line;
	ifstream file("Resources\\Font.txt");

	if (file.is_open()) {
		char *endChar;
		// Get number of lines in file
		getline(file, line);
		numberOfFontChars = strtol(line.c_str(), &endChar, 10);
		fontStructure = new FontStructure*[numberOfFontChars];
		
		for (int i = 0; i < numberOfFontChars; i++) {
			fontStructure[i] = new FontStructure;
			getline(file, line);
			// Copy data from file to font structure
			//id
			size_t startPos = line.find("id")+3;
			size_t toCopy = line.find(" ", startPos + 1) - startPos;
			string data = line.substr(startPos, toCopy);
			fontStructure[i]->id = strtol(data.c_str(), &endChar, 10);

			//x
			startPos = line.find("x", startPos) + 2;
			toCopy = line.find(" ", startPos + 1) - startPos;
			data = line.substr(startPos, toCopy);
			fontStructure[i]->x = strtol(data.c_str(), &endChar, 10);
			//y
			startPos = line.find("y", startPos) + 2;
			toCopy = line.find(" ", startPos + 1) - startPos;
			data = line.substr(startPos, toCopy);
			fontStructure[i]->y = strtol(data.c_str(), &endChar, 10);

			//width
			startPos = line.find("width", startPos) + 6;
			toCopy = line.find(" ", startPos + 1) - startPos;
			data = line.substr(startPos, toCopy);
			fontStructure[i]->width = strtol(data.c_str(), &endChar, 10);

			//height
			startPos = line.find("height", startPos) + 7;
			toCopy = line.find(" ", startPos + 1) - startPos;
			data = line.substr(startPos, toCopy);
			fontStructure[i]->height = strtol(data.c_str(), &endChar, 10);

			//xoffset
			startPos = line.find("xoffset", startPos) + 8;
			toCopy = line.find(" ", startPos + 1) - startPos;
			data = line.substr(startPos, toCopy);
			fontStructure[i]->xOffset = strtol(data.c_str(), &endChar, 10);

			//yoffset
			startPos = line.find("yoffset", startPos) + 8;
			toCopy = line.find(" ", startPos + 1) - startPos;
			data = line.substr(startPos, toCopy);
			fontStructure[i]->yOffset = strtol(data.c_str(), &endChar, 10);

			//xadvance
			startPos = line.find("xadvance", startPos) + 9;
			toCopy = line.find(" ", startPos + 1) - startPos;
			data = line.substr(startPos, toCopy);
			fontStructure[i]->xAdvance = strtol(data.c_str(), &endChar, 10);
		}
		file.close();
	}
	else {
		return false;
	}

	return true;
}

// Draw string
void DrawString(int destX, int destY, const char *text, bool centerX) {
	int stringLength = strlen(text);

	// If centering is requested, calculate where to print string
	if (centerX) {
		int stringLengthInPixels = 0;
		// Calculate length of string in pixels
		for (int i = 0; i < stringLength; i++) {
			char c = text[i];
			for (int j = 0; j < numberOfFontChars; j++) {
				if (fontStructure[j]->id == (int)c) {
					stringLengthInPixels += (fontStructure[j]->xAdvance);
					break;
				}
			}
		}
		// Calculate starting position for string
		destX = (int)(screenWidth / 2 / scaling - stringLengthInPixels / 2);
	}

	// Print string
	for (int i = 0; i < stringLength; i++) {
		char c = text[i];

		for (int j = 0; j < numberOfFontChars; j++) {
			if (fontStructure[j]->id == (int)c) {
				D3DXVECTOR2 destination((float)destX + fontStructure[j]->xOffset, (float)destY + fontStructure[j]->yOffset);

				DrawChar(destination, fontStructure[j]->x, fontStructure[j]->y, fontStructure[j]->width, fontStructure[j]->height, scaling);
				destX += (fontStructure[j]->xAdvance);
				break;
			}
		}
	}
}