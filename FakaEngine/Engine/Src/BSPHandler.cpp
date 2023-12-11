#include "BSPHandler.h"
#include "Entity.h"
#include "Mesh.h"
#include "Utils/Utils.h"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
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
void BSPHandler::CheckObjectsInBsp(Entity* node, bool isRoot)
{
	Mesh* currMesh = static_cast<Mesh*>(node);
	if (currMesh != NULL)
	{
		if (!isRoot)
		{
			bool checkPassed = true;

			glm::vec3* aabPositions = currMesh->GetAABBGlobalPositions();

			for (int i = 0; i < _logicPlanes_BSP.size(); i++)
			{
				if (!_logicPlanes_BSP[i]->ValidateObject(aabPositions[1] /*Posicion Minima del AABB*/, aabPositions[7] /*Posicion Maxima del AABB*/))
				{
					checkPassed = false;
					break;
				}
			}

			currMesh->SetIsAlive(checkPassed);
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
		_cameraPlaneChecks[i] = _logicPlanes_BSP[i]->GetSide(_camera->transform.position);
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

void BSPHandler::FlipBspPlane(int index)
{
	if (_logicPlanes_BSP.size() >= index){
		return;
	}

	_logicPlanes_BSP[index]->GetLogicPlane()->FlipPlane();
	_logicPlanes_BSP[index]->UpdateShapeReference();
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
		_logicPlanes_BSP[i]->DrawShapeReference(wireFrameEnable);
	}
}
#pragma endregion