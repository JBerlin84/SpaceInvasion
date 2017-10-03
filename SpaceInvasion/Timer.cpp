#include "Timer.h"

Timer::Timer() {
	restart();
}

void Timer::restart() {
	start = chrono::system_clock::now();
}

double Timer::getTime() {
	chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
	
	chrono::duration<double> elapsed = end - start;
	return elapsed.count();
}