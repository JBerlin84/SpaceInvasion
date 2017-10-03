/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Main game file, updates, draws and read controllers.
************************************************************************/

#include <windows.h>

extern int gameBoardWidth;	// used for coordination system, loosely based on 4k-resolution
extern int gameBoardHeight;

bool GameInit(HWND hwnd);
void GameTick(HWND hwnd);
void GameDisplay(HWND hwnd);
void GameEnd(HWND hwnd);