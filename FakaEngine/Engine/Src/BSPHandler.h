#ifndef BSPHANDLER
#define BSPHANDLER

#include <vector>
#include <map>

#include "PrivateClass/Export.h"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

#include "AxisAlignedBoundingBox.h"
#include "Plane.h"
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

	void CheckObjectsInBsp(Entity* node, bool isRoot);
public:

	BSPHandler(Entity* _camera, Entity* _rootScene);
	~BSPHandler();

	void AddPlaneBSP(PlaneBSP* newBsp);
	void ValidateObjectInBsp(ModelNode* root);
	void ValidateCameraInBsp();

	bool ValidateEntityInBspPlanes(Entity* node);

	void SetNewPlaneMesh(ModelNode* planeNode, string planeName);
	void DrawBSPMeshes(bool& wireFrameEnable);

	vector<PlaneBSP*> GetLogicBspPlanes() { return _logicPlanes_BSP; }
	int GetAmountBSP_Planes() { return _countBSP_Planes; }
	vector<BSPNodeData> GetBSP_PlanesData() { return _bspNodes; }
	Entity* GetRootScene() { return _rootScene; }

protected:
	vector<bool> _cameraPlaneChecks;
};
#endif // !BSPHANDLER