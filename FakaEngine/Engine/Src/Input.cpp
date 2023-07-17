#include "Input.h"
#include "glew.h"
#include "GLFW/glfw3.h"

Input::Input(GLFWwindow *_contextWindows) {contextWindows = _contextWindows;}

//=============================================
Input::~Input(){}
//=============================================
//bool Input::GetKeyDown(KeyBoard _keyBoard)
//{
//	return CheckKeyPress(_keyBoard, GLFW_REPEAT);	<-----------------	NO FUNCA ARREGLAR MAS TARDE MASTER
//}
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
void Input::SetContextWindows(GLFWwindow *_contextWindows)
{
	contextWindows = _contextWindows;
}
//=============================================
bool Input::CheckKeyPress(KeyBoard _keyBoard, unsigned int type)
{
	return (glfwGetKey(contextWindows, _keyBoard) == type);
}
//=============================================