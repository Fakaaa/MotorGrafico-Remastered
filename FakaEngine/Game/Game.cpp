#include "Game.h"
#include "Camera.h"
#include "Light.h"
#include "model.h"
#include "BSPHandler.h"

#include <vector>

#include <stdlib.h>

#include "Gui/EngineGui.h"

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

Game::~Game() {}

#pragma endregion

#pragma region OVERRIDES

void Game::InitGame()
{
	cout << "Init of game" << endl;
	InitCustomCamera();
	InitCustomLights();
	InitOfMaterials();

	_bspScene = new Model(_renderer, _bspHandler);
	_bspScene->LoadModel("Res/Models/BSPScene.fbx", "");
	_bspScene->SetMaterial(_textureMaterialForLight);
	_bspScene->SetName("BSP_Scene");
	_bspScene->SetPosition(670, 30, 850);
	_bspScene->SetScale(30.0f, 30.0f, 30.0f);
	_bspScene->SetRotationX(-90.0f);
	_bspScene->SetRotationZ(-90.0f);

	AddObjectInDenugGame(_bspScene);

	for (int i = 0; i < _bspHandler->GetBSP_PlanesData().size(); i++)
	{
		Entity* bspNode = _bspHandler->GetBSP_PlanesData()[i].node;
		//bspNode->SetInmortalObject(true);
		bspNode->SetPosition(670, 30, 850);
		bspNode->SetScale(100.0f, 100.0f, 100.0f);
		bspNode->SetRotationX(-90.0f);
		bspNode->SetRotationZ(-90.0f);
	}

	objectsToComputeInBSP.push_back(_bspScene);
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

	if (_testModel != NULL)
	{
		_testModel->Draw(_engineGUI->GetIfWireFrameIsActive());
	}

	if (_bspScene != NULL)
	{
		_bspScene->Draw(_engineGUI->GetIfWireFrameIsActive());
	}

	if (_mainCamera != NULL)
	{
		ControlCamera(_input);
	}

	if (_bspHandler != NULL)
	{
		_bspHandler->UpdateObjectsRecursiveCommon(objectsToComputeInBSP);
		_bspHandler->DrawBSPMeshes(_engineGUI->GetIfWireFrameIsActive());
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

	if (_testModel != NULL)
	{
		_testModel->UnloadModel();
		delete _testModel;
		_testModel = NULL;
	}

	if (_bspScene != NULL)
	{
		_bspScene->UnloadModel();
		delete _bspScene;
		_bspScene = NULL;
	}
}

#pragma endregion

#pragma region PRIVATE_METHODS

void Game::ControlCamera(Input* input)
{
#pragma region CAMERA MOVE
	float speed = 0;
	//INPUTS
	KeyBoard moveUpCamera = KeyBoard::KEY_SPACE;
	KeyBoard moveDownCamera = KeyBoard::KEY_LEFT_CONTROL;
	KeyBoard moveLeftCamera = KeyBoard::KEY_A;
	KeyBoard moveRightCamera = KeyBoard::KEY_D;
	KeyBoard moveForwardCamera = KeyBoard::KEY_S;
	KeyBoard moveBackCamera = KeyBoard::KEY_W;

	//ROTACION EN Z NO DISPONIBLE AUN.
	KeyBoard y_rotateLeft = KeyBoard::KEY_J;
	KeyBoard y_rotateRight = KeyBoard::KEY_L;
	KeyBoard x_rotateLeft = KeyBoard::KEY_I;
	KeyBoard x_rotateRight = KeyBoard::KEY_K;

	float lastCameraPtch = _mainCamera->GetPitch();
	float lastCameraYaw = _mainCamera->GetYaw();

	if (input->GetKey(KEY_TAB))
	{
		input->SetUseMouseCamera(true);
	}
	else if (input->GetKey(KEY_ESCAPE))
	{
		input->SetUseMouseCamera(false);
	}

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

void Game::InitCustomLights()
{
	AddLight(Light::Point, 0);
	SetLightPosition(0, glm::vec3(1016.0f, -77.0f, 310.0));
	ChangeColorLight(0, cyan);
	AddObjectInDenugGame(GetLight(0));

	AddLight(Light::Point, 1);
	SetLightPosition(1, glm::vec3(120, -81.0f, 385.0));
	ChangeColorLight(1, red);
	AddObjectInDenugGame(GetLight(1));

	AddLight(Light::Point, 2);
	SetLightPosition(2, glm::vec3(567, -56.0f, -619.0));
	ChangeColorLight(2, orange);
	AddObjectInDenugGame(GetLight(2));

	AddLight(Light::Spot, 3);
	SetLightPosition(3, glm::vec3(625, -123, 502));
	ChangeColorLight(3, magenta);
	AddObjectInDenugGame(GetLight(3));

	AddLight(Light::Directional, 4);
	SetLightPosition(4, glm::vec3(16, 645, 300));
	ChangeColorLight(4, glm::vec3(1.0));
	AddObjectInDenugGame(GetLight(4));
}

void Game::InitCustomCamera()
{
	newPositionCamX = 670.5;
	newPositionCamY = 155;
	newPositionCamZ = 1815;

	_mainCamera->SetPosition(glm::vec3(newPositionCamX, newPositionCamY, newPositionCamZ));

	_mainCamera->SetUseFrustrum(false);
	_mainCamera->UseFrustrum(_window->GetAspectRatio());
	_mainCamera->SetEnableDrawAABB(true);
	_mainCamera->SetRotationY(270);
	AddObjectInDenugGame(_mainCamera);
}

void Game::InitTestEngine(bool status)
{
	_triangle = new Shape(_renderer, TypeShape::TRIANGLE);
	_triangle->BindGeneralData();

	_triangle->SetName("Test_Triangle");
	_triangle->SetPosition(883.0f, 71.0f, 355.0);
	_triangle->SetScale(120.0f, 120.0f, 120.0f);
	_triangle->SetNewMaterial(_goldMaterial);

	_cube = new Primitive3D(_renderer, TypeModel::Cube, "Res/Textures/ZoroProfile.jpg", false);
	_cube->SetName("Test Cube");
	_cube->SetPosition(847.0f, -12.0f, 250.0);
	_cube->SetScale(690.0f, 20.0f, 815.0f);
	_cube->SetNewMaterial(_goldMaterial);

	_testModel = new Model(_renderer, _bspHandler);
	_testModel->LoadModel("Res/Models/NewTank/tank.obj", "Res/Models/NewTank/");
	_testModel->SetMaterial(_textureMaterialForLight);
	_testModel->SetName("TankModel");
	_testModel->SetPosition(1170, 30, 850);
	_testModel->SetScale(30.0f, 30.0f, 30.0f);
	_testModel->SetRotationY(160.0f);

	AddObjectInDenugGame(_triangle);
	AddObjectInDenugGame(_cube);
	AddObjectInDenugGame(_testModel);
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