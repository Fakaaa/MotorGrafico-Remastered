#ifndef TIME_H
#define TIME_H
#include "PrivateClass/Export.h"
#include <chrono>
#include <time.h>

class ENGINE_API Time {
	std::chrono::system_clock::time_point _startTime;
	//std::chrono::duration<float> _deltaTime;
	float _deltaTime;
	float _timeScale;
	float _elapsedTime = 0;
	float _framesCounter;
	int frames;
	float fps = 0;
public:
	Time();
	~Time();
	void reset();
	void setTimeScale(float t);
	float getTimeScale();
	float getSeconds();
	void startTime();
	void tick();
	float deltaTime();
	void calculateFps();
	float getFPS();
	void FPS();
};
#endif // !TIME_H