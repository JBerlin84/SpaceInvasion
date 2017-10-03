/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
Win32 main
************************************************************************/

#include <Windows.h>
#include <fstream>
#include <stdlib.h>

#include "DirectX.h"
#include "Game.h"

string configFile = "Resources\\Config.cfg";

bool  fullscreen;
int   screenWidth;
int   screenHeight;
float scaling;