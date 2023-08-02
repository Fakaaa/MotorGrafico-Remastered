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

	for (int i = 0; i < planes_BSP.size(); i++)
	{
		if (planes_BSP[i] != NULL) 
		{
			delete planes_BSP[i];
			planes_BSP[i] = NULL;
		}
	}

	planes_BSP.clear();
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
		
		for (int j = 0; j < planes_BSP.size(); j++)
		{
			if (!planes_BSP[j]->CheckObjectInPlaneBSP(objects[i]))
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

		for (int j = 0; j < planes_BSP.size(); j++)
		{
			if (!planes_BSP[j]->CheckObjectInPlaneBSP(objects[i]))
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
	planes_BSP.push_back(newBsp);
}

#pragma endregion