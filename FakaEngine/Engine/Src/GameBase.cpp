#include "GameBase.h"
#include "../Lib/GLEW/Include/glew.h"
#include "../Lib/GLFW/include/GLFW/glfw3.h"

#pragma region CONSTRUCTOR

GameBase::GameBase() {}
GameBase::~GameBase() {}

#pragma endregion

#pragma region PUBLIC_METHODS

int GameBase::InitEngine() 
{
	bool initGLFW = glfwInit();

	window = new Window(1080, 680, "Faka's Engine");

	//PRUEBA DEL MATERIAL (SPECULAR)
	if (!initGLFW || window == NULL)
		return INIT_ERROR;

	return 0;
}

void GameBase::UpdateEngine()
{
}

void GameBase::DestroyEngine()
{
}

void GameBase::DisableObjectInGame(Entity* entity)
{
}

void GameBase::EnableObjectInGame(Entity* entity)
{
}
#pragma endregion