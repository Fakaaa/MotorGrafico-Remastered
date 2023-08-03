#ifndef BSPHANDLER
#define BSPHANDLER

#include <vector>
#include <map>

#include "PrivateClass/Export.h"

#include "PlaneBSP.h"
#include "Entity.h"
#include "ModelNode.h"

class ENGINE_API BSPHandler
{
private:
	Entity* _camera;
	Entity* _rootScene;

	struct BSPNodeData
	{
		ModelNode* node;
		string name;
	};

	int _countBSP_Planes = 0;

	vector<PlaneBSP*> _logicPlanes_BSP;
	vector<BSPNodeData> _bspNodes;
public:
	BSPHandler(Entity* _camera, Entity* _rootScene);
	~BSPHandler();

	bool UpdateObjectsRecursiveInverse(vector<Entity*> objects);
	void UpdateObjectsRecursiveCommon(vector<Entity*> objects);
	void AddPlaneBSP(PlaneBSP* newBsp);

	void SetNewPlaneMesh(ModelNode* planeNode, string planeName);
	void DrawBSPMeshes(bool& wireFrameEnable);

	int GetAmountBSP_Planes() { return _countBSP_Planes; }
	vector<BSPNodeData> GetBSP_PlanesData() { return _bspNodes; }
	Entity* GetRootScene() { return _rootScene; }
};
#endif // !BSPHANDLER