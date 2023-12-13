#include "BSPHandler.h"
#include "Entity.h"
#include "Mesh.h"
#include "Utils/Utils.h"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

#include "AxisAlignedBoundingBox.h"
#include "Plane.h"

#include <math.h>
#include <vector>

#pragma region CONSTRUCTOR

BSPHandler::BSPHandler(Entity* _camera, Entity* _rootScene)
{
	this->_camera = _camera;
	this->_rootScene = _rootScene;
}

BSPHandler::~BSPHandler()
{
	_camera = NULL;
	_rootScene = NULL;

	for (int i = 0; i < _logicPlanes_BSP.size(); i++)
	{
		if (_logicPlanes_BSP[i] != NULL)
		{
			delete _logicPlanes_BSP[i];
			_logicPlanes_BSP[i] = NULL;
		}
	}

	_logicPlanes_BSP.clear();

	for (int i = 0; i < _bspNodes.size(); i++)
	{
		if (_bspNodes[i].node != NULL)
		{
			delete _bspNodes[i].node;
			_bspNodes[i].node = NULL;
		}
	}

	_bspNodes.clear();

	_countBSP_Planes = 0;
}
#pragma endregion

#pragma region PRIVATE_METHODS
bool BSPHandler::ValidateEntityInBspPlanes(Entity* node)
{
	bool checkPassed = true;

	for (int i = 0; i < _logicPlanes_BSP.size(); i++)
	{
		glm::vec3 aabbPos0 = node->GetAABBSidesFromExtent()[0];
		glm::vec3 aabbPos1 = node->GetAABBSidesFromExtent()[1];
		glm::vec3 aabbPos2 = node->GetAABBSidesFromExtent()[2];

		float dotProd0 = _logicPlanes_BSP[i]->GetDistanceToPoint(aabbPos0);
		float dotProd1 = _logicPlanes_BSP[i]->GetDistanceToPoint(aabbPos1);
		float dotProd2 = _logicPlanes_BSP[i]->GetDistanceToPoint(aabbPos2);

		if (dotProd0 < 0.0f && dotProd1 < 0.0f && dotProd2 < 0.0f)
		{
			checkPassed = false;
			break;
		}
	}

	return checkPassed;
}

void BSPHandler::CheckObjectsInBsp(Entity* node, bool isRoot)
{
	Mesh* currMesh = static_cast<Mesh*>(node);
	if (currMesh != NULL)
	{
		if (ValidateEntityInBspPlanes(node))
		{
			currMesh->SetIsAlive(true);
		}
		else {
			currMesh->SetIsAlive(false);
		}
	}

	for (int i = 0; i < node->GetChildrens().size(); i++)
	{
		CheckObjectsInBsp(node->GetChildrens()[i], false);
	}
}
#pragma endregion

#pragma region PUBLIC_METHODS
void BSPHandler::AddPlaneBSP(PlaneBSP* newBsp)
{
	_logicPlanes_BSP.push_back(newBsp);
	_cameraPlaneChecks.push_back(false);
}

void BSPHandler::ValidateObjectInBsp(ModelNode* root)
{
	CheckObjectsInBsp(root, true);
}

void BSPHandler::ValidateCameraInBsp()
{
	for (int i = 0; i < _logicPlanes_BSP.size(); i++)
	{
		_cameraPlaneChecks[i] = !(_logicPlanes_BSP[i]->GetDistanceToPoint(_camera->transform.position) < 0.0f);
	}

	/*cout << "----------Camera plane sides----------" << endl;
	for (int i = 0; i < _cameraPlaneChecks.size(); i++)
	{
		if (_cameraPlaneChecks[i])
		{
			cout << "Plane " << i << " POSITIVE" << endl;
		}
		else {
			cout << "Plane " << i << " NEGATIVE" << endl;
		}
	}
	cout << "--------------------------------------" << endl;*/
}

void BSPHandler::SetNewPlaneMesh(ModelNode* planeNode, string planeName)
{
	BSPNodeData newbspDrawData;

	planeNode->SetupAxisAlignedBoundingBox();

	newbspDrawData.node = planeNode;
	newbspDrawData.name = planeName;

	_bspNodes.push_back(newbspDrawData);
	_countBSP_Planes++;
}

void BSPHandler::DrawBSPMeshes(bool& wireFrameEnable)
{
	for (int i = 0; i < _bspNodes.size(); i++)
	{
		if (_bspNodes[i].node != NULL)
		{
			_bspNodes[i].node->Draw(wireFrameEnable);
		}
	}

	for (int i = 0; i < _logicPlanes_BSP.size(); i++)
	{
		bool wireFrameActive = true;
		_logicPlanes_BSP[i]->DrawShapeReference(wireFrameActive);
	}
}
#pragma endregion