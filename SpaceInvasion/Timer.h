/************************************************************************
Author: Jimmy Berlin
Last modified: 2015-08-23
Project: Introduction to game programming in windows, project assignment

desc:
a timer giving time since last restart in seconds.
************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>

using namespace std;

class Timer {
public:
	Timer();
	void restart();
	double getTime();
private:
	chrono::time_point<chrono::system_clock> start;
};

#endif
