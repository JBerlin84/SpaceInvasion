/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Manages the state of an animation, holds a pointer to the actual animation
frames.
************************************************************************/

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <vector>
#include <fstream>
#include "DirectX.h"
#include "Timer.h"

using namespace std;

class AnimationFrames {
public:
	AnimationFrames(string folder, string filename);
	bool loadAnimation(string folder, string filename);
	D3DXVECTOR2 loadSize(string folder, string filename);
	vector<LPDIRECT3DTEXTURE9> getTextureVector();
	D3DXVECTOR2 getSize();
private:
	vector<LPDIRECT3DTEXTURE9> frames;
	D3DXVECTOR2 size;
};

class Animation {
public:
	Animation();
	Animation(AnimationFrames* frames, int fps = 60, bool loop = false, D3DXVECTOR2 position = D3DXVECTOR2(0, 0), float scale = 1);
	Animation(string folder, string filename, int fps, bool loop = false, D3DXVECTOR2 position = D3DXVECTOR2(0, 0), float scale = 1);
	void storeFramesPointer(AnimationFrames *frames);
	void storeSize(const char *file);
	LPDIRECT3DTEXTURE9 getFrame();
	D3DXVECTOR2 getPosition();
	D3DXVECTOR2 getSize();
	float getScale();
	void start();
	bool hasPlayedOnce();
private:
	AnimationFrames *frames;
	Timer *timer;
	bool playedOnce;
	bool loop;
	int fps;
	D3DXVECTOR2 position;
	D3DXVECTOR2 size;
	float relativeScale;
};

#endif