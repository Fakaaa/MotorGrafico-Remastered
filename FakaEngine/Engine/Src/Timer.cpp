#include "Timer.h"
#include <iostream>

Time::Time() {
	reset();
	_timeScale = 1.0f;
	//_deltaTime = std::chrono::duration<float>(0.0f);
	_deltaTime = 0;
}
Time::~Time() {

}

void Time::reset() {
	_startTime = std::chrono::system_clock::now();
}
double oldT = clock();
void Time::FPS() {
	std::cout <<"FPS: "<< 1.0f / deltaTime() << std::endl;
}
float Time::deltaTime() {
	return _deltaTime;
}

void Time::setTimeScale(float t) {
	_timeScale = t;
}
float Time::getTimeScale() {
	return _timeScale;
}

float Time::getSeconds() {
	return 0;
}

void Time::startTime() {
}
void Time::tick() {
	double t = clock();
	_deltaTime = (float)((t - oldT) / 1000.0f);
	oldT = t;
}
void Time::calculateFps() {
	_elapsedTime += deltaTime();
	frames++;
	if (_elapsedTime >= 1.0f) {
		fps = frames / _elapsedTime;
		frames = 0;
		_elapsedTime = 0;
	}
}
float Time::getFPS() {
	return fps;
}