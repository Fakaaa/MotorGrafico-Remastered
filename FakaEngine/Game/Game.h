#ifndef GAME_H
#define GAME_H

#include "GameBase.h"
#include "Window.h"
#include "Entity.h"
#include "Input.h"
#include "Shape.h"
#include "model.h"

class Game : public GameBase
{
private:
	void ControlCamera(Input* input);
	void InitCustomLights();
	void InitCustomCamera();
	void InitTestEngine(bool status);

	Shape* _triangle = NULL;
	Primitive3D* _cube = NULL;
	Model* _testModel = NULL;
	Model* _bob = NULL;

	Material* _goldMaterial = NULL;

	bool _mouseCameraControl;

	void InitOfMaterials();
public:
	Game();
	~Game();

	void InitGame() override;
	void UpdateGame(Window* _window, Renderer* _renderer, Input* _input) override;
	void DestroyGame() override;
};
#endif // !GAME_H