#include "Game.h"
#include "Camera.h"
#include "Light.h"
#include <stdlib.h>

using namespace std;

#pragma region PRIVATE_VARIABLES
float speedMovementCamera = 600.0f;
float speedRotateCamera = 90.0f;

float newPositionCamX = 0;
float newPositionCamY = 0;
float newPositionCamZ = 0;

glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 purple = glm::vec3(0.6784f, 0.0157f, 0.9412f);
glm::vec3 yellow = glm::vec3(1.0f, 0.9843f, 0.0f);
glm::vec3 brown = glm::vec3(0.4392f, 0.3059f, 0.1333f);
glm::vec3 orange = glm::vec3(1.0f, 0.5686f, 0.0f);
glm::vec3 cyan = glm::vec3(0.0157f, 0.9412f, 0.9412f);
glm::vec3 magenta = glm::vec3(0.9412f, 0.0157f, 0.7412f);
#pragma endregion


#pragma region CONSTRUCTOR

Game::Game() : GameBase() {}

Game::~Game(){}

#pragma endregion

#pragma region OVERRIDES

void Game::InitGame()
{
	cout << "Init of game" << endl;

	newPositionCamX = 1230.5;
	newPositionCamY = 115;
	
	_mainCamera->SetPosition(glm::vec3(-375.4, 85, 0));
	_mainCamera->SetInmortalObject(true);
	_mainCamera->SetRotationX(-15);
	_mainCamera->SetRotationY(171);
	AddObjectInDenugGame(_mainCamera);

	AddLight(Light::Point, 0);
	SetLightPosition(0, glm::vec3(195, -56.0f, -75.0));
	ChangeColorLight(0, cyan);
	AddObjectInDenugGame(GetLight(0));

	InitOfMaterials();

	_triangle = new Shape(_renderer, TypeShape::TRIANGLE);
	_triangle->BindGeneralData();
	
	_triangle->SetName("Test_Triangle");
	_triangle->SetPosition(300.0f, 100.0f, 10.0);
	_triangle->SetRotationX(0);
	_triangle->SetRotationY(90);
	_triangle->SetRotationZ(0);
	_triangle->SetScale(120.0f, 120.0f, 120.0f);
	_triangle->SetNewMaterial(_goldMaterial);

	_cube = new Primitive3D(_renderer, TypeModel::Cube);
	_cube->SetName("Test Cube");
	_cube->SetPosition(630.0f, -154.0f, -105.0);
	_cube->SetScale(120.0f, 120.0f, 120.0f);

	AddObjectInDenugGame(_triangle);
	AddObjectInDenugGame(_cube);
}

void Game::UpdateGame(Window* _window, Renderer* _renderer, Input* _input)
{
	if (_triangle != NULL)
	{
		_triangle->Draw(_engineGUI->GetIfWireFrameIsActive());
	}

	if (_cube != NULL)
	{
		_cube->Draw(_engineGUI->GetIfWireFrameIsActive());
	}

	if (_mainCamera != NULL)
	{
		ControlCamera(_input);
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

	if (_cube != NULL)
	{
		delete _cube;
		_cube = NULL;
	}
}

#pragma endregion

#pragma region PRIVATE_METHODS

void Game::ControlCamera(Input* input)
{
#pragma region CAMERA MOVE
	float speed = 0;
	//INPUTS
	KeyBoard moveUpCamera = KeyBoard::KEY_W;
	KeyBoard moveDownCamera = KeyBoard::KEY_S;
	KeyBoard moveLeftCamera = KeyBoard::KEY_A;
	KeyBoard moveRightCamera = KeyBoard::KEY_D;
	KeyBoard moveForwardCamera = KeyBoard::KEY_UP;
	KeyBoard moveBackCamera = KeyBoard::KEY_DOWN;

	//ROTACION EN Z NO DISPONIBLE AUN.
	KeyBoard y_rotateLeft = KeyBoard::KEY_J;
	KeyBoard y_rotateRight = KeyBoard::KEY_L;
	KeyBoard x_rotateLeft = KeyBoard::KEY_I;
	KeyBoard x_rotateRight = KeyBoard::KEY_K;

	//TRASLACION
	if (input->GetKey(moveUpCamera))
	{
		newPositionCamY += speedMovementCamera * _timeClock.deltaTime();
	}
	if (input->GetKey(moveDownCamera))
	{
		newPositionCamY -= speedMovementCamera * _timeClock.deltaTime();
	}
	if (input->GetKey(moveLeftCamera))
	{
		newPositionCamX -= speedMovementCamera * _timeClock.deltaTime();
	}
	if (input->GetKey(moveRightCamera))
	{
		newPositionCamX += speedMovementCamera * _timeClock.deltaTime();
	}
	if (input->GetKey(moveForwardCamera))
	{
		newPositionCamZ += speedMovementCamera * _timeClock.deltaTime();
	}
	if (input->GetKey(moveBackCamera))
	{
		newPositionCamZ -= speedMovementCamera * _timeClock.deltaTime();
	}

	if (input->GetKey(KeyBoard::KEY_LEFT))
	{
		_mainCamera->ChangePerspective(TypeProjectionCamera::Ortho);
	}
	if (input->GetKey(KeyBoard::KEY_LEFT))
	{
		_mainCamera->ChangePerspective(TypeProjectionCamera::Perspective);
	}

	//ROTACION
	if (input->GetKey(y_rotateLeft))
	{
		speed = speedRotateCamera * _timeClock.deltaTime();
		_mainCamera->RotateCameraY(speed);
	}
	if (input->GetKey(y_rotateRight))
	{
		speed = speedRotateCamera * _timeClock.deltaTime();
		_mainCamera->RotateCameraY(-speed);
	}
	if (input->GetKey(x_rotateLeft))
	{
		speed = speedRotateCamera * _timeClock.deltaTime();
		_mainCamera->RotateCameraX(speed);

	}
	if (input->GetKey(x_rotateRight))
	{
		speed = speedRotateCamera * _timeClock.deltaTime();
		_mainCamera->RotateCameraX(-speed);
	}

	_mainCamera->SetPosition(newPositionCamX, newPositionCamY, newPositionCamZ);
#pragma endregion
}

void Game::InitOfMaterials()
{
	_goldMaterial = new Material();
	_goldMaterial->SetAmbientMat(glm::vec3(0.24725f, 0.1995f, 0.0745f));
	_goldMaterial->SetDiffuseMat(glm::vec3(0.75164f, 0.60648f, 0.22648f));
	_goldMaterial->SetSpecularMat(glm::vec3(0.628281f, 0.555802f, 0.366065f));
	_goldMaterial->SetNewShininess(0.1f, 128);
}

#pragma endregion