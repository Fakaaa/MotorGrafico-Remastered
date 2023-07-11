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
	return 0;
}

void GameBase::UpdateEngine()
{
}

void GameBase::DestroyEngine()
{
}

#pragma endregion