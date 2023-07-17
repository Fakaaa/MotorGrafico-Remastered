#include "Animation.h"
#include <iostream>

Animation::Animation()
{
	_currentTime = 0;
	_currentFrame = 0;
	_length = 200;
}

Animation::~Animation()
{
	if(_animations.size() > 0)
		_animations.clear();
	if (_totalFrames.size() > 0)
		_totalFrames.clear();
}

void Animation::Update(Time& timer)
{
	_currentTime += (timer.deltaTime());
	
	//std::cout << _currentTime <<std::endl;
	while (_currentTime >= _length){
		_currentTime -= _length;
		//std::cout << "NDEAAAAh" << std::endl;
	}

	 float frameLength = _length / _animations[_currentAnimation].size();
	_currentFrame = static_cast<int>(_currentTime / frameLength);

	//std::cout << "CurrenTime frame anim: "<<_currentFrame << std::endl;
}
void Animation::AddFrame(float frameX, float frameY, int spriteWidth, int spriteHeigth, int textureWidth, int textureHeigth, float timeToAnim, int totalFrames, int countFramesForFilas)
{
	_length = timeToAnim;

	cout << "total frames" << totalFrames << endl;
	totalFrames = totalFrames + countFramesForFilas;
	float index_X = 0;
	float index_Y = 0;
	Frame frame;
	cout << "total frames" << totalFrames << endl;
	for (int i = 0; i < totalFrames; i++) {
		//--------
		frame.frameCoords[0].U = ((frameX + index_X) / textureWidth);
		frame.frameCoords[0].V = ((spriteHeigth + index_Y) / textureHeigth);
		//--------
		//cout << frame.frameCoords[0].U << endl;
		//cout << frame.frameCoords[0].V << endl;

		frame.frameCoords[1].U = (((frameX + index_X) + spriteWidth) / textureWidth);
		frame.frameCoords[1].V = ((spriteHeigth + index_Y) / textureHeigth);

		//cout << frame.frameCoords[1].U << endl;
		//cout << frame.frameCoords[1].V << endl;
		//--------
		frame.frameCoords[2].U = (((frameX + index_X) + spriteWidth) / textureWidth);
		frame.frameCoords[2].V = ((frameY + index_Y) / textureHeigth);

		//cout << frame.frameCoords[2].U << endl;
		//cout << frame.frameCoords[2].V << endl;
		//--------
		frame.frameCoords[3].U = ((frameX + index_X) / textureWidth);
		frame.frameCoords[3].V = ((frameY + index_Y) / textureHeigth);

		//cout << frame.frameCoords[3].U << endl;
		//cout << frame.frameCoords[3].V << endl;
		//--------
		_totalFrames.push_back(frame);
		index_X += spriteWidth;
		if (i > 0)
		{
			if (i % countFramesForFilas == 0)
			{
				index_Y += spriteHeigth;
				_animations.push_back(_totalFrames);
				_totalFrames.clear();
			}
		}
	}
	_animations[0].resize(countFramesForFilas);

	//for (int i = 0; i < _animations.size(); i++) 
	//{
	//	cout << "TAM ANIM " << i<<":" << _animations[i].size() << endl;
	//}
}
void Animation::AddFrame(float frameX, float frameY, int spriteWidth, int spriteHeigth, int textureWidth, int textureHeigth, float timeToAnim)
{
	_length = timeToAnim;

	Frame frame;
	
	frame.frameCoords[0].U = ((frameX) / textureWidth);
	frame.frameCoords[0].V = ((spriteHeigth + frameY) / textureHeigth);
	//--------
	//cout << frame.frameCoords[0].U << endl;
	//cout << frame.frameCoords[0].V << endl;

	frame.frameCoords[1].U = (((frameX) + spriteWidth) / textureWidth);
	frame.frameCoords[1].V = ((spriteHeigth + frameY) / textureHeigth);

	//cout << frame.frameCoords[1].U << endl;
	//cout << frame.frameCoords[1].V << endl;
	//--------
	frame.frameCoords[2].U = (((frameX) + spriteWidth) / textureWidth);
	frame.frameCoords[2].V = ((frameY) / textureHeigth);

	//cout << frame.frameCoords[2].U << endl;
	//cout << frame.frameCoords[2].V << endl;
	//--------
	frame.frameCoords[3].U = ((frameX) / textureWidth);
	frame.frameCoords[3].V = ((frameY) / textureHeigth);

	//cout << frame.frameCoords[3].U << endl;
	//cout << frame.frameCoords[3].V << endl;
	//--------
	_totalFrames.push_back(frame);
}

void Animation::AddAnimation() 
{
	if (_totalFrames.size() > 0) 
	{
		//cout << "PUSHIE LA ANIMACION VIEJA" << endl;
		_animations.push_back(_totalFrames);
		_totalFrames.clear();
		//cout << _animations.size() << endl;
	}
}
int Animation::GetCurrentFrame()
{
	return _currentFrame;
}

vector<Frame>& Animation::GetAnimation()
{
	if(_currentAnimation < _animations.size())
		return _animations[_currentAnimation];

	return _animations[_animations.size()-1];
}
