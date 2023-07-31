#include "Input.h"
#include "glew.h"
#include "GLFW/glfw3.h"

//Camera control
bool _controlCameraWithMouse;
bool _firstMouse;
float _lastX;
float _lastY;
float _yaw;
float _pitch;
glm::vec3 _cameraLook;

Camera* _camera;

void MouseCallaback(GLFWwindow* _window, double xpos, double ypos);

Input::Input(GLFWwindow* _contextWindows, Camera* _contextCamera)
{
	this->_contextWindows = _contextWindows;
	_camera = _contextCamera;
	glfwSetCursorPosCallback(_contextWindows, MouseCallaback);
}

//=============================================
Input::~Input() {}
//=============================================
bool Input::GetKey(KeyBoard _keyBoard)
{
	return CheckKeyPress(_keyBoard, GLFW_PRESS);
}
//=============================================
bool Input::GetKeyUp(KeyBoard _keyBoard)
{
	return CheckKeyPress(_keyBoard, GLFW_RELEASE);
}
//=============================================
void Input::SetContextWindows(GLFWwindow* _contextWindows)
{
	this->_contextWindows = _contextWindows;
}
void Input::SetUseMouseCamera(bool status)
{
	_controlCameraWithMouse = status;

	if (_controlCameraWithMouse)
	{
		glfwSetInputMode(_contextWindows, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(_contextWindows, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
//=============================================
bool Input::CheckKeyPress(KeyBoard _keyBoard, unsigned int type)
{
	return (glfwGetKey(_contextWindows, _keyBoard) == type);
}
//=============================================
void MouseCallaback(GLFWwindow* _window, double xpos, double ypos)
{
	if (!_controlCameraWithMouse)
	{
		return;
	}

	if (_firstMouse)
	{
		_lastX = xpos;
		_lastY = ypos;
		_firstMouse = false;
	}

	float xoffset = xpos - _lastX;
	float yoffset = _lastY - ypos;
	_lastX = xpos;
	_lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_yaw += xoffset;
	_pitch += yoffset;

	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	direction.y = sin(glm::radians(_pitch));
	direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_cameraLook = glm::normalize(direction);

	_camera->SetFront(_cameraLook);
}
//=============================================