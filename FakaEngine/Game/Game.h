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

	Shape* _triangle = NULL;
	Primitive3D* _cube = NULL;
	Model* _testModel = NULL;

	Material* _goldMaterial = NULL;

	void InitOfMaterials();
public:
	Game();
	~Game();

	void InitGame() override;
	void UpdateGame(Window* _window, Renderer* _renderer, Input* _input) override;
	void DestroyGame() override;
};
#endif // !GAME_H