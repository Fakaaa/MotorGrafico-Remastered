#include "Window.h"
#include "glew.h"
#include "GLFW/glfw3.h"

GLFWwindow* RefWindow;

//============================================
Window::Window()
{
	RefWindow = glfwCreateWindow(640, 480, "Na ventanita", NULL, NULL);
	SetSize(640, 480);
}
//============================================
Window::Window(int x, int y, const char* name)
{
	RefWindow = glfwCreateWindow(x, y, name, NULL, NULL);
	SetSize(x, y);
}
//============================================
Window::~Window()
{
	glfwDestroyWindow(RefWindow);
	glfwTerminate();
}
//============================================
void Window::SwapBuffersWindows()
{
	glfwSwapBuffers(RefWindow);
}
//============================================
void Window::SetSize(int _sizeX, int _sizeY)
{
	sizeX = _sizeX;
	sizeY = _sizeY;
	//EJECUTAR FUNCION QUE SETEA LOS VALORES DE LA PANTALLA.
}
//============================================
int Window::GetSizeX()
{
	return sizeX;
}
//============================================
int Window::GetSizeY()
{
	return sizeY;
}
//============================================
void Window::SetNameWindows(char name[])
{
	//SETEARIA EL NOMBRE DE LA VENTANA.
}
//============================================
void Window::SetFullscreen(bool _fullscreen)
{
	//HARIA QUE LA PANTALLA SEA FULL.
}
//============================================
GLFWwindow* Window::GetWindowsPtr()
{
	return RefWindow;
}
//============================================
int Window::CheckCreateWindows()
{
	if (!RefWindow)
	{
		glfwTerminate();
		return -1;
	}
}
//============================================
void Window::CreateContextWindows()
{
	glfwMakeContextCurrent(RefWindow);

	glfwSwapInterval(1);
}
//============================================
bool Window::CheckGLFWwindowShouldClose()
{
	return glfwWindowShouldClose(RefWindow);
}
//============================================