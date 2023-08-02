#ifndef BSPHANDLER
#define BSPHANDLER

#include <vector>
#include <map>

#include "PrivateClass/Export.h"

#include "PlaneBSP.h"
#include "Entity.h"

class ENGINE_API BSPHandler
{
private:
	Entity* _camera;
	Entity* _rootScene;

public:
	BSPHandler(Entity* _camera, Entity* _rootScene);
	~BSPHandler();

	bool UpdateObjectsRecursiveInverse(vector<Entity*> objects);
	void UpdateObjectsRecursiveCommon(vector<Entity*> objects);
	void AddPlaneBSP(PlaneBSP* newBsp);

	vector<PlaneBSP*> planes_BSP;
};
#endif // !BSPHANDLER