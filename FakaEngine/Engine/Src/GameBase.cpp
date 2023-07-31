#include "../Lib/GLEW/Include/glew.h"
#include "../Lib/GLFW/include/GLFW/glfw3.h"

#include "GameBase.h"
#include "Camera.h"
#include "Material.h"
#include "CollisionManager.h"
#include "Gui/EngineGui.h"

vector<Entity*> GameBase::entitysDebugInGame = vector<Entity*>();

#pragma region CONSTRUCTOR

GameBase::GameBase() {}
GameBase::~GameBase() {}

#pragma endregion

#pragma region PUBLIC_METHODS

int GameBase::InitEngine()
{
	if (!InitializeEngine())
		return INIT_ERROR;

	ConfigureRenderer();
	ConfigureCamera();

	glEnable(GL_DEPTH_TEST);

	return 0;
}

void GameBase::UpdateEngine()
{
	while (!_window->CheckGLFWwindowShouldClose())
	{
		_renderer->BeignDraw();
		_timeClock.tick();

		if (_useGUI)
		{
			_engineGUI->NewFrame();
			_engineGUI->TreeEntitys(_rootScene);
			_engineGUI->SetCountObjectInScreen(0);
			_engineGUI->CheckCountObjectsInScreen(_rootScene);
			_engineGUI->Text("Objetos en pantalla: " + to_string(_engineGUI->GetCountObjectInScreen() - 1));
		}

		HandleCamera();
		HandleLight();

		//Virtualizacion, para que el game desde afuera pueda ejuctar lo que requiera en su Update
		UpdateGame(_window, _renderer, _input);

		if (_useGUI)
		{
			_engineGUI->Render();
		}

		glfwPollEvents();

		_renderer->EndDraw(_window);
	}
}

void GameBase::DestroyEngine()
{
	glfwTerminate();

	if (_engineGUI != NULL)
	{
		delete _engineGUI;
		_engineGUI = NULL;
	}

	if (_input != NULL)
	{
		delete _input;
		_input = NULL;
	}

	if (_window != NULL)
	{
		delete _window;
		_window = NULL;
	}

	if (_renderer != NULL)
	{
		delete _renderer;
		_renderer = NULL;
	}

	if (_mainCamera != NULL)
	{
		delete _mainCamera;
		_mainCamera = NULL;
	}

	while (_lights.size() > 0)
	{
		if (_lights[0] != NULL)
		{
			delete _lights[0];
		}
		_lights.erase(_lights.begin());
	}
}

void GameBase::AddObjectInDenugGame(Entity* entity)
{
	if (entity != NULL)
	{
		for (int i = 0; i < entitysDebugInGame.size(); i++)
		{
			if (entitysDebugInGame[i] == entity)
			{
				return;
			}
		}
		entitysDebugInGame.push_back(entity);
		_rootScene->AddChildren(entity);

		if (entity->GetName() != "MainCamera")
			AddObjectInFrustrumCulling(entity);
	}
}

void GameBase::RemoveObjectInDebugGame(Entity* entity)
{
	int index = -1;
	for (int i = 0; i < entitysDebugInGame.size(); i++)
	{
		if (entitysDebugInGame[i] == entity)
		{
			entitysDebugInGame[i]->SetShowInDebug(false);
			index = i;
			i = elementsForVertexCount;
		}
	}
	if (index != -1) {
		entitysDebugInGame.erase(entitysDebugInGame.begin() + index);
		_rootScene->RemoveChildren(entity, _rootScene);
	}
}

void GameBase::AddObjectInFrustrumCulling(Entity* entity)
{
	bool pushEnableFrustrumCulling = true;

	for (int i = 0; i < _mainCamera->objectsCheckFrustrum.size(); i++)
	{
		if (_mainCamera->objectsCheckFrustrum[i] == entity)
			pushEnableFrustrumCulling = false;
	}

	if (pushEnableFrustrumCulling)
		_mainCamera->objectsCheckFrustrum.push_back(entity);

	for (Entity* child : entity->GetChildrens()) {
		AddObjectInFrustrumCulling(child);
	}
}

void GameBase::AddLight(Light::TypeLight typeLight, int id)
{
	for (int i = 0; i < _lights.size(); i++)
	{
		if (_lights[i] != NULL)
		{
			if (_lights[i]->GetMyId() == id)
			{
				std::cout << "La luz[" << i << "] del vector ya pose el identificador: " << id << std::endl;
				return;
			}
		}
	}
	Light* newLight = new Light(_renderer, typeLight, _mainCamera);
	if (typeLight == Light::Directional)
		newLight->SetAmbient(glm::vec3(0.5f, 0.5f, 0.5f));
	else
		newLight->SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));

	newLight->SetDiffuse(glm::vec3(0.2f, 0.2f, 0.2f));
	newLight->SetSpecular(glm::vec3(1.5f, 1.5f, 1.5f));

	newLight->SetIdLight(id);

	newLight->SetPosition(350.0f, 200.0f, 300.0f);
	newLight->SetScale(10.0f, 10.0f, 10.0f);

	stringstream ss;
	ss << id;
	string index = ss.str();

	switch (newLight->GetTypeLight())
	{
	case Light::TypeLight::Directional:
		_renderer->SetLighting(newLight, Light::nr_of_directional_light - 1);

		newLight->SetName("Directional Light " + index);
		break;
	case Light::TypeLight::Point:
		_renderer->SetLighting(newLight, Light::nr_of_point_light - 1);
		newLight->SetName("Point Light " + index);
		break;
	case Light::TypeLight::Spot:
		_renderer->SetLighting(newLight, Light::nr_of_spot_light - 1);
		newLight->SetName("Spot Light " + index);
		break;
	}
	cout << "Count pointLight: " << Light::nr_of_point_light << endl;
	cout << "Count directionalLight: " << Light::nr_of_directional_light << endl;
	cout << "Count spotLight: " << Light::nr_of_spot_light << endl;

	_lights.push_back(newLight);
}

Light* GameBase::GetLight(int id)
{
	for (int i = 0; i < _lights.size(); i++)
	{
		if (_lights[i]->GetMyId() == id)
		{
			return _lights[i];
		}
	}
	return NULL;
}

void GameBase::RemoveLight(int id)
{
	for (int i = 0; i < _lights.size(); i++)
	{
		if (_lights[i] != NULL)
		{
			if (_lights[i]->GetMyId() == id) {
				delete _lights[i];
				_lights.erase(_lights.begin() + i);
				i = _lights.size();
			}
		}
	}
}

void GameBase::SetLightPosition(int id, glm::vec3 position)
{
	for (int i = 0; i < _lights.size(); i++)
	{
		if (_lights[i] != NULL)
		{
			if (_lights[i]->GetMyId() == id)
			{
				_lights[i]->SetPosition(_lights[i]->transform.position.x + position.x,
					_lights[i]->transform.position.y + position.y, _lights[i]->transform.position.z + position.z);

				i = _lights.size();
			}
		}
	}
}

void GameBase::SetTypeLightDefault(int id, Light::TypeLight setType)
{
	for (int i = 0; i < _lights.size(); i++)
	{
		if (_lights[i] != NULL)
		{
			if (_lights[i]->GetMyId() == id)
			{
				_lights[i]->SetTypeLightDefault(setType);
				system("cls");
				cout << "Count pointLight: " << Light::nr_of_point_light << endl;
				cout << "Count directionalLight: " << Light::nr_of_directional_light << endl;
				cout << "Count spotLight: " << Light::nr_of_spot_light << endl;
			}
		}
	}
}

void GameBase::ChangeColorLight(int id, glm::vec3 color)
{
	for (int i = 0; i < _lights.size(); i++)
	{
		if (_lights[i] != NULL)
		{
			if (_lights[i]->GetMyId() == id)
			{
				_lights[i]->SetColour(color);
			}
		}
	}
}

void GameBase::DisableObjectInGame(Entity* entity)
{
	entity->SetIsAlive(false);
}

void GameBase::EnableObjectInGame(Entity* entity)
{
	entity->SetIsAlive(true);
}
#pragma endregion

#pragma region PRIVATE_METHODS

bool GameBase::InitializeEngine()
{
	bool initGLFW = glfwInit();

	_window = new Window(1440, 980, "Faka's Engine");
	_renderer = new Renderer();
	_mainCamera = new Camera(_renderer, TypeProjectionCamera::Perspective);
	_input = new Input(_window->GetWindowsPtr(), _mainCamera);

	_engineGUI = new EngineGui(_window);

	_rootScene = new EmptyObject(_renderer);
	_rootScene->SetName("RootObjects");

	return initGLFW && _window != NULL;
}

void GameBase::ConfigureRenderer()
{
	_window->CheckCreateWindows();
	_window->CreateContextWindows();
	_renderer->GLEWInit();
	_renderer->SetCurrentShaderUse("../Engine/Res/Shaders/Vertex.shader", "../Engine/Res/Shaders/FragmentColor.shader");

	_textureMaterialForLight = new Material();
	_textureMaterialForLight->SetAmbientMat(glm::vec3(0.5f, 0.5f, 0.5f));
	_textureMaterialForLight->SetDiffuseMat(glm::vec3(0.0f, 0.0f, 0.0f));
	_textureMaterialForLight->SetSpecularMat(glm::vec3(0.5f, 0.5f, 0.5f));
	_textureMaterialForLight->SetNewShininess(1.0f, 128.0f);

	_textureMaterialDefault = new Material();
	_textureMaterialDefault->SetAmbientMat(glm::vec3(0.0f, 0.0f, 0.0f));
	_textureMaterialDefault->SetDiffuseMat(glm::vec3(0.0f, 0.0f, 0.0f));
	_textureMaterialDefault->SetSpecularMat(glm::vec3(0.0f, 0.0f, 0.0f));
	_textureMaterialDefault->SetNewShininess(0.25f, 128.0f);
}

void GameBase::ConfigureCamera()
{
	_mainCamera->SetDataOrtho(0.0f, _window->GetSizeX(), 0.0f, _window->GetSizeY(), _nearPlane, _farPlane);
	_mainCamera->SetDataPerspective(_fovAmount, _window->GetSizeX(), _window->GetSizeY(), _nearPlane, _farPlane);
	_mainCamera->UseProjection();

	_mainCamera->SetPosition(300.0f, 100.0f, 1000.0f);

	_mainCamera->InitCamera(_mainCamera->transform.position, glm::vec3(0.0f, 1.0f, 0.0f), -90, 0);
	_mainCamera->SetViewFirstPerson();
	_mainCamera->SetName("MainCamera");

	_mainCamera->ChangePerspective(TypeProjectionCamera::Perspective);
	_mainCamera->ChangeActualFrustrum();

	_mainCamera->SetEnableDrawAABB(false);
}

void GameBase::ConfigureOtherUtils()
{
}

void GameBase::HandleCamera()
{
	switch (_mainCamera->GetTypeCamera())
	{
	case TypeCamera::FirstPerson:
		_mainCamera->SetViewFirstPerson();
		break;
	case TypeCamera::ThirdPerson:
		_mainCamera->SetViewThirdPerson();
		break;
	}
	_mainCamera->UseFrustrum();
	_mainCamera->UseCamera(_renderer->GetCurrentShaderUse(), _mainCamera->GetInternalData().localModel);
}

void GameBase::HandleLight()
{
	for (int i = 0; i < _lights.size(); i++)
	{
		if (_lights[i] != NULL)
		{
			_lights[i]->Draw(_engineGUI->GetIfWireFrameIsActive());
		}
	}
}

#pragma endregion