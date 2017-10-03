#include "Animation.h"

AnimationFrames::AnimationFrames(string folder, string filename) {
	string line;
	ifstream file(folder + filename);

	if (file.is_open()) {
		while (getline(file, line)) {
			frames.push_back(LoadTexture(folder + line));
		}
		file.close();
		size = loadSize(folder, filename);
	}
	else {
		return;
	}

	return;
}

bool AnimationFrames::loadAnimation(string folder, string filename) {

	string line;
	ifstream file(folder + filename);

	if (file.is_open()) {
		while (getline(file, line)) {
			frames.push_back(LoadTexture(folder + line));
		}
		file.close();
		size = loadSize(folder, filename);
	}
	else {
		return false;
	}

	return true;
}

D3DXVECTOR2 AnimationFrames::loadSize(string folder, string filename) {

	D3DXVECTOR2 size;

	string line;
	ifstream file(folder + filename);

	if (file.is_open()) {
		getline(file, line);
		size = GetBitmapSize(folder + line);
		file.close();
	}

	return size;
}

vector<LPDIRECT3DTEXTURE9> AnimationFrames::getTextureVector() {
	return frames;
}

D3DXVECTOR2 AnimationFrames::getSize() {
	return size;
}


Animation::Animation() {
	playedOnce = false;
	timer = new Timer();
	fps = 30;
}

Animation::Animation(AnimationFrames* frames, int fps, bool loop, D3DXVECTOR2 position, float scale) {
	this->frames = frames;
	size = frames->getSize();

	playedOnce = false;
	timer = new Timer();
	this->fps = fps;
	this->frames = frames;
	this->loop = loop;
	this->position = position;
	relativeScale = scale;
}


Animation::Animation(string folder, string filename, int fps, bool loop, D3DXVECTOR2 position, float scale) {
	frames = new AnimationFrames(folder, filename);
	size = frames->getSize();

	playedOnce = false;
	timer = new Timer();
	this->fps = fps;
	this->frames = frames;
	this->loop = loop;
	relativeScale = scale;
}

void Animation::storeFramesPointer(AnimationFrames *frames) {
	this->frames = frames;
}

void Animation::storeSize(const char *file) {
	this->size = GetBitmapSize(file);
}

void Animation::start() {
	timer->restart();
	playedOnce = false;
}

bool Animation::hasPlayedOnce() {
	return playedOnce;
}

D3DXVECTOR2 Animation::getPosition() {
	return position;
}

D3DXVECTOR2 Animation::getSize() {
	return size;
}

float Animation::getScale() {
	return relativeScale;
}

LPDIRECT3DTEXTURE9 Animation::getFrame() {
	double time = timer->getTime();
	int frameNo;
	
	double animationLength = frames->getTextureVector().size() * 1.0 / fps;				// calculate the length of the animation
	if (time < animationLength) {
		frameNo = (int)(time / animationLength * frames->getTextureVector().size());	// Calculate the correct frame to resend
	}
	else {
		playedOnce = true;
		frameNo = frames->getTextureVector().size() - 1;								// If we have viewed the complete animation, send the last frame over and over again.
		if (loop) {																		// If the animation should loop, simply restart timer.
			timer->restart();
		}
	}
	return frames->getTextureVector()[frameNo];
}