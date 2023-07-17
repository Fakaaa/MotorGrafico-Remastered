#ifndef GAMEBASE_H
#define GAMEBASE_H

#include "PrivateClass/Export.h"
#include "Window.h"
#include "Entity.h"
#include <stdlib.h>

#define INIT_ERROR -1

class ENGINE_API GameBase 
{
private:
protected:
	Window* window = NULL;

public:
	GameBase();
	~GameBase();

	static void DisableObjectInGame(Entity* entity);
	static void EnableObjectInGame(Entity* entity);

	int InitEngine();
	void UpdateEngine();
	void DestroyEngine();
};
#endif