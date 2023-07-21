#ifndef GAMEBASE_H
#define GAMEBASE_H

#include "PrivateClass/Export.h"
#include "Gui/EngineGui.h"

#include "Window.h"
#include "Entity.h"
#include "Input.h"
#include "Timer.h"
#include "Light.h"
#include "EmptyObject.h"

#include <stdlib.h>

#define INIT_ERROR -1

class ENGINE_API GameBase 
{
private:
	vector<Light*> _lights;

	void HandleCamera();
	void HandleLight();

	EmptyObject* _rootScene = NULL;
	
protected:
	Window* _window = NULL;
	Renderer* _renderer = NULL;
	Camera* _mainCamera = NULL;
	Input* _input = NULL;
	Time _timeClock;
	EngineGui* _engineGUI = NULL;
public:
	GameBase();
	~GameBase();

	static void DisableObjectInGame(Entity* entity);
	static void EnableObjectInGame(Entity* entity);

	int InitEngine();
	void UpdateEngine();
	void DestroyEngine();

	virtual void InitGame() = 0;
	virtual void UpdateGame(Window* _window, Renderer* _render, Input* _input) = 0;
	virtual void DestroyGame() = 0;
};
#endif