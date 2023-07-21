#include "../Lib/GLEW/Include/glew.h"
#include "../Lib/GLFW/include/GLFW/glfw3.h"

#include "GameBase.h"
#include "Camera.h"

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

void GameBase::DisableObjectInGame(Entity* entity)
{
}

void GameBase::EnableObjectInGame(Entity* entity)
{
}
#pragma endregion

#pragma region PRIVATE_METHODS

bool GameBase::InitializeEngine()
{
	bool initGLFW = glfwInit();

	_window = new Window(1080, 680, "Faka's Engine");
	_renderer = new Renderer();
	_input = new Input(_window->GetWindowsPtr());
	_mainCamera = new Camera(_renderer, TypeProjectionCamera::Ortho);

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
	_renderer->SetCurrentShaderUse("", "");
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