#include "Game.h"
#include <stdlib.h>

using namespace std;

#pragma region CONSTRUCTOR

Game::Game() : GameBase() {}

Game::~Game(){}

#pragma endregion

#pragma region OVERRIDES

void Game::InitGame()
{
	cout << "Init of game" << endl;

	_triangle = new Shape(_renderer, TypeShape::TRIANGLE);
	//_triangle->BindGeneralData();

	_triangle->SetName("Test_Triangle");
	_triangle->SetPosition(-190.0f, 200.0f, 10.0);
	_triangle->SetScale(120.0f, 120.0f, 120.0f);
	_triangle->SetNewMaterial(_textureMaterialForLight);

	AddObjectInDenugGame(_triangle);
}

void Game::UpdateGame(Window* _window, Renderer* _renderer, Input* _input)
{
	if (_triangle != NULL)
	{
		_triangle->Draw(_wireFrameEnable);
	}
}

void Game::DestroyGame() 
{
	cout << "Destroy of game" << endl;

	if (_triangle != NULL)
	{
		delete _triangle;
		_triangle = NULL;
	}
}

#pragma endregion

#pragma region PRIVATE_METHODS

void Game::ControlCamera()
{

}

#pragma endregion