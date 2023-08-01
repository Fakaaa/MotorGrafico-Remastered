#ifndef GAMEBASE_H
#define GAMEBASE_H

#include "PrivateClass/Export.h"

#include "Window.h"
#include "Entity.h"
#include "Input.h"
#include "Timer.h"
#include "Light.h"
#include "Primitive3D.h"
#include "EmptyObject.h"

#include <stdlib.h>

#define INIT_ERROR -1

class EngineGui;

class ENGINE_API GameBase
{
private:
	vector<Light*> _lights;

	bool InitializeEngine();
	void ConfigureRenderer();
	void ConfigureCamera();
	void ConfigureOtherUtils();

	void HandleCamera();
	void HandleLight();

	bool _useGUI = true;

	EmptyObject* _rootScene = NULL;

	const float _nearPlane = 100.0f;
	const float _farPlane = -10000.0f;
	const float _fovAmount = 45.0f;
protected:

	static vector<Entity*> entitysDebugInGame;

	Window* _window = NULL;
	Renderer* _renderer = NULL;
	Camera* _mainCamera = NULL;
	Input* _input = NULL;
	Time _timeClock;
	EngineGui* _engineGUI = NULL;

	Material* _textureMaterialForLight = NULL;
	Material* _textureMaterialDefault = NULL;

public:
	void AddObjectInDenugGame(Entity* entity);
	void RemoveObjectInDebugGame(Entity* entity);

	void AddObjectInFrustrumCulling(Entity* entity);

	GameBase();
	~GameBase();

	static void DisableObjectInGame(Entity* entity);
	static void EnableObjectInGame(Entity* entity);

	Light* GetLight(int id);
	void AddLight(Light::TypeLight typeLight, int id);
	void RemoveLight(int id);
	void SetLightPosition(int id, glm::vec3 position);
	void SetTypeLightDefault(int id, Light::TypeLight setType);
	void ChangeColorLight(int id, glm::vec3 color);

	int InitEngine();
	void UpdateEngine();
	void DestroyEngine();

	virtual void InitGame() = 0;
	virtual void UpdateGame(Window* _window, Renderer* _render, Input* _input) = 0;
	virtual void DestroyGame() = 0;
};
#endif