#ifndef ANIMATION_H
#define ANIMATION_H

#include "PrivateClass/Export.h"
#include "Timer.h"
#include "Frame.h"
#include <vector>

using namespace std;

class ENGINE_API Animation
{
private:
	int _currentFrame;
	float _currentTime;
	float _length;
	int _acumulationWidth;
	int _acumulationHeighth;
	vector<Frame> _totalFrames;
	vector<vector<Frame>> _animations;
	int _currentAnimation = 0;
public:
	Animation();
	~Animation();
	void Update(Time& timer);
	void AddFrame(float frameX, float frameY, int spriteWidth, int spriteHeigth, int textureWidth, int textureHeigth, float timeToAnim, int totalFrames, int countFilas);
	int GetCurrentFrame();
	vector<Frame>& GetAnimation();
	void SetCurrentAnimation(int currentAnimation) { _currentAnimation = currentAnimation; }
	void AddFrame(float frameX, float frameY, int spriteWidth, int spriteHeigth, int textureWidth, int textureHeigth, float timeToAnim);
	void AddAnimation();
};

#endif