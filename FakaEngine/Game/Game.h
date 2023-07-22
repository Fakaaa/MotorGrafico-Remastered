#ifndef GAME_H
#define GAME_H

#include "GameBase.h"
#include "Window.h"
#include "Entity.h"
#include "Input.h"
#include "Shape.h"

class Game : public GameBase 
{
private:
	void ControlCamera();

	Shape* _triangle = NULL;
	bool _wireFrameEnable = false;
public:
	Game();
	~Game();

	void InitGame() override;
	void UpdateGame(Window* _window, Renderer* _renderer, Input* _input) override;
	void DestroyGame() override;
};
#endif // !GAME_H