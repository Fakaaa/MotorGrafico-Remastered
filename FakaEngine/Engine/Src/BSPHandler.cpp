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
void BSPHandler::ValidateEntityInBspPlanes(Entity* node, vector<bool>& nodeChecks)
{
	nodeChecks.clear();

	for (int i = 0; i < _logicPlanes_BSP.size(); i++)
	{
		glm::vec3 aabbPos0 = node->GetAABBGlobalPositions()[0];
		glm::vec3 aabbPos7 = node->GetAABBGlobalPositions()[7];

		float dotProd0 = _logicPlanes_BSP[i]->GetDistanceToPoint(aabbPos0);
		float dotProd7 = _logicPlanes_BSP[i]->GetDistanceToPoint(aabbPos7);

		if (dotProd0 < 0.0f && dotProd7 < 0.0f)
		{
			nodeChecks.push_back(false);
		}
		else {
			nodeChecks.push_back(true);
		}
	}
}

bool BSPHandler::UpdatedValidateObjectsBsp(Entity* node)
{
	bool checkPassed = true;
	
	ModelNode* modelNode = static_cast<ModelNode*>(node);
	if (modelNode != NULL && !modelNode->isMesh)
	{
		for (int i = 0; i < _logicPlanes_BSP.size(); i++)
		{
			glm::vec3 aabbPos0 = node->GetAABBGlobalPositions()[1] - node->GetAABB()->GetFixedMinExtent();
			glm::vec3 aabbPos7 = node->GetAABBGlobalPositions()[7] + node->GetAABB()->GetFixedMaxExtent();

			float dotProd0 = _logicPlanes_BSP[i]->GetDistanceToPoint(aabbPos0);
			float dotProd7 = _logicPlanes_BSP[i]->GetDistanceToPoint(aabbPos7);

			if (_cameraPlaneChecks[i])
			{
				if (dotProd0 < 0.0f && dotProd7 < 0.0f)
				{
					checkPassed = false;
				}
			}
			else
			{
				if (dotProd0 > 0.0f && dotProd7 > 0.0f)
				{
					checkPassed = false;
				}
			}
		}
	}
	else {
		checkPassed = modelNode->GetParent()->GetIsAlive();
	}

	return checkPassed;
}

void BSPHandler::CheckObjectsInBsp(Entity* node)
{
	bool checkPassed = UpdatedValidateObjectsBsp(node);

	if (checkPassed)
	{
		node->SetIsAlive(true);
	}
	else {
		node->SetIsAlive(false);
	}

	for (int i = 0; i < node->GetChildrens().size(); i++)
	{
		CheckObjectsInBsp(node->GetChildrens()[i]);
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
	CheckObjectsInBsp(root);
}

void BSPHandler::ValidateCameraInBsp()
{
	for (int i = 0; i < _logicPlanes_BSP.size(); i++)
	{
		_cameraPlaneChecks[i] = !(_logicPlanes_BSP[i]->GetDistanceToPoint(_camera->transform.position) < 0.0f);
	}
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