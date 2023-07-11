#ifndef GAMEBASE_H
#define GAMEBASE_H

#include "PrivateClass/Export.h"
#include <stdlib.h>

class ENGINE_API GameBase 
{
private:
protected:
public:
	GameBase();
	~GameBase();

	int InitEngine();
	void UpdateEngine();
	void DestroyEngine();
};
#endif