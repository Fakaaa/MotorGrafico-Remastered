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
}

void Game::UpdateGame(Window* _window, Renderer* _renderer, Input* _input)
{

}

void Game::DestroyGame() 
{
	cout << "Destroy of game" << endl;
}

#pragma endregion

#pragma region PRIVATE_METHODS

void Game::ControlCamera()
{

}

#pragma endregion