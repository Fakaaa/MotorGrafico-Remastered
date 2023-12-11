#ifndef GAME_H
#define GAME_H

#include "GameBase.h"
#include "Window.h"
#include "Entity.h"
#include "Input.h"
#include "Shape.h"
#include "model.h"
#include "PlaneBSP.h"

#include <vector>

class Game : public GameBase
{
private:
	void ControlCamera(Input* input);
	void InitCustomLights();
	void InitCustomCamera();
	void InitTestEngine(bool status);

	Shape* _triangle = NULL;
	Primitive3D* _shapeReference = NULL;
	Model* _testModel = NULL;
	Model* _bspScene = NULL;

	Material* _goldMaterial = NULL;

	vector<Entity*> objectsToComputeInBSP;

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