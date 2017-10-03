/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
DirectX functions, modified version of the one given in:
"Beginning game programming" by Jonathan S. Harbour
************************************************************************/

#pragma once;
#define DIRECTINPUT_VERSION 0x0800
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <ctime>		// Adds time.h and adds the assoicated names to the std namespace
#include <iostream>
#include <fstream>
#include <sstream>
#include <d3d9.h>
#include <dinput.h>
#include <iomanip>
#include <d3dx9.h>
#include "DirectSound.h"

using namespace std;

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")

// Win32/program variables
extern const string TITLE;
extern int          screenWidth;
extern int          screenHeight;
extern bool         fullscreen;
extern float        scaling;
extern bool         gameOver;

// DirectInput
extern LPDIRECTINPUT8       dinput;
extern LPDIRECTINPUTDEVICE8 mouse;
extern LPDIRECTINPUTDEVICE8 keyboard;
extern DIMOUSESTATE         mouseState;

bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
int  KeyDown(int);
int  MouseButton(int);
int  MouseX();
int  MouseY();

// Direct3D
const float PI = 3.14159f;

extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;
extern LPD3DXSPRITE spriteobj;
extern LPDIRECT3DTEXTURE9 fontTexture;			// TODO: use hashmap instead?
extern int numberOfFontChars;
extern LPDIRECT3DSURFACE9 loadingScreen;

bool Direct3D_Init(HWND hwnd, int *width, int *height, bool fullscreen);
void Direct3D_Shutdown();
bool LoadFont();
LPDIRECT3DSURFACE9 LoadSurface(string filename);
void DrawSurface(LPDIRECT3DSURFACE9 dest, float xPos, float yPos, LPDIRECT3DSURFACE9 source);
void DrawBackground(LPDIRECT3DSURFACE9 source);
void DrawLoading();
D3DXVECTOR2 GetBitmapSize(string filename);
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor = NULL);
void DrawChar(D3DXVECTOR2 destination, int x, int y, int width, int height, float scaling);
void DrawTexture(float xPos, float yPos, LPDIRECT3DTEXTURE9 texture);
void DrawString(int destX, int destY, const char *text, bool centerX = false);
void DrawSprite(LPDIRECT3DTEXTURE9 sprite, D3DXVECTOR2 destination, D3DXVECTOR2 size, float scaling = 1, float rotation = 0, int frameNumber = 0, int columns = 1, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

// DirectSound
extern CSoundManager *dSound;

bool DirectSound_Init(HWND hwnd);
void DirectSound_Shutdown();
CSound *LoadSound(string filename);
void PlaySound(CSound *sound);
void LoopSound(CSound *sound);
void StopSound(CSound *sound);

// Structures
struct Texture {
	LPDIRECT3DTEXTURE9 texture;
	D3DXVECTOR2 size;
	float rotation = 0;
};

struct FontStructure {
	int id;
	int x, y;
	int width, height;
	int xOffset, yOffset;
	int xAdvance;
};