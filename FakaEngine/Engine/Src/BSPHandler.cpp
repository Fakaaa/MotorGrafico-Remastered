#include "BSPHandler.h"
#include "Entity.h"
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

#pragma endregion

#pragma region PUBLIC_METHODS

bool BSPHandler::UpdateObjectsRecursiveInverse(vector<Entity*> objects)
{
	bool result = false;

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetChildrens().size() != 0)
		{
			if (UpdateObjectsRecursiveInverse(objects[i]->GetChildrens()))
			{
				objects[i]->SetIsAlive(true);
				continue;
			}
		}

		bool canBeActive = true;
		
		for (int j = 0; j < _logicPlanes_BSP.size(); j++)
		{
			if (!_logicPlanes_BSP[j]->CheckObjectInPlaneBSP(objects[i]))
			{
				canBeActive = false;
				break;
			}
		}

		if (!canBeActive)
		{
			objects[i]->SetIsAlive(false);
		}
		else 
		{
			result = true;
			objects[i]->SetIsAlive(true);
		}
	}

	return result;
}

void BSPHandler::UpdateObjectsRecursiveCommon(vector<Entity*> objects)
{
	for (int i = 0; i < objects.size(); i++)
	{
		bool canBeActive = true;

		for (int j = 0; j < _logicPlanes_BSP.size(); j++)
		{
			if (!_logicPlanes_BSP[j]->CheckObjectInPlaneBSP(objects[i]))
			{
				canBeActive = false;
				break;
			}
		}

		if (!canBeActive)
		{
			if (objects[i]->GetIsAlive())
			{
				objects[i]->DisableMeAndChilds();
			}
		}
		else 
		{
			if (!objects[i]->GetIsAlive()) 
			{
				objects[i]->EnableMeAndChilds();
			}
			UpdateObjectsRecursiveCommon(objects[i]->GetChildrens());
		}
	}
}

void BSPHandler::AddPlaneBSP(PlaneBSP* newBsp)
{
	newBsp->SetCurrentCameraCompare(_camera);
	_logicPlanes_BSP.push_back(newBsp);
}

void BSPHandler::SetNewPlaneMesh(ModelNode* planeNode, string planeName)
{
	BSPNodeData newbspDrawData;

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
}

#pragma endregion