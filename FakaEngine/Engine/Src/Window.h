#ifndef WINDOWS_H
#define WINDOWS_H
#define TAM 100

#include "PrivateClass/Export.h"

struct GLFWwindow;

class ENGINE_API Window {
private:
	int sizeX;
	int sizeY;
	char nameWindows[TAM];
	bool fullscreen;
public:
	Window();
	Window(int x, int y, const char* name);
	~Window();
	void SetSize(int _sizeX, int _sizeY);
	int GetSizeX();
	int GetSizeY();
	float GetAspectRatio();
	void SetNameWindows(char name[]);
	void SetFullscreen(bool _fullscreen);
	GLFWwindow* GetWindowsPtr();
	int CheckCreateWindows();
	void CreateContextWindows();
	bool CheckGLFWwindowShouldClose();
	//void ClearWindows();
	void SwapBuffersWindows();
	//void EventsWindows();
};
#endif