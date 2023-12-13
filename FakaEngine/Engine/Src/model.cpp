#include "Model.h"

#include <glew.h>
#include <GLFW/glfw3.h>

#include "ModelImporter.h"
#include "PlaneBSP.h"

#include "ModelNode.h"
#include "Utils/Utils.h"

#include "AxisAlignedBoundingBox.h"

#include "Camera.h"

Model::Model(Renderer* render, BSPHandler* bspHandler) : Entity(render)
{
	myMat = NULL;
	rootNode = NULL;
	this->bspHandler = bspHandler;
	modelImporter = new ModelImporter(bspHandler);
}

Model::~Model()
{
	if (modelImporter != NULL)
		delete modelImporter;

	if (!myMat)
		delete myMat;

	UnloadModel();
}

void Model::LoadModel(const string& filePath, const string& texturePath)
{
	if (modelImporter != NULL)
	{
		rootNode = modelImporter->LoadModel(modelMeshes, filePath, texturePath, rootNode, modelChildrens, textureList, renderer);
	}

	if (rootNode != NULL) 
	{
		AddChildren(rootNode);
		rootNode->_textureList = textureList;
	}

	for (int i = 0; i < modelChildrens.size(); i++)
	{
		if (modelChildrens[i] != NULL)
		{
			modelChildrens[i]->_textureList = textureList;
		}
	}

	for (int i = 0; i < bspHandler->GetBSP_PlanesData().size(); i++)
	{
		if (bspHandler->GetBSP_PlanesData()[i].node != NULL) {
			rootNode->RemoveChildren(bspHandler->GetBSP_PlanesData()[i].node, bspHandler->GetRootScene());
		}
	}

	if (rootNode != NULL)
	{
		rootNode->SetupAxisAlignedBoundingBox();
	}

	for (int i = 0; i < modelChildrens.size(); i++)
	{
		if (modelChildrens[i] != NULL)
		{
			modelChildrens[i]->SetupAxisAlignedBoundingBox();
		}
	}
}

void Model::Draw(bool& wireFrameActive)
{
	if (isAlive || InmortalObject)
	{
		if (rootNode != NULL)
			rootNode->Draw(wireFrameActive);

		for (int i = 0; i < modelChildrens.size(); i++)
		{
			if (modelChildrens[i] != NULL)
				modelChildrens[i]->Draw(wireFrameActive);
		}
	}
}

void Model::UnloadModel()
{
	if (rootNode != NULL)
	{
		delete rootNode;
		rootNode = NULL;
	}

	for (int i = 0; i < modelChildrens.size(); i++)
	{
		if (modelChildrens[i] != NULL)
		{
			delete modelChildrens[i];
			modelChildrens[i] = NULL;
		}
	}
	for (int i = 0; i < textureList.size(); i++)
	{
		if (textureList[i] != NULL)
		{
			delete textureList[i];
			textureList[i] = NULL;
		}
	}
}

string Model::GetNameOfClass()
{
	return "Model";
}

void Model::SetMaterial(Material* mat)
{
	myMat = mat;

	if (rootNode != NULL)
		rootNode->SetMaterialNode(myMat);

	for (int i = 0; i < modelChildrens.size(); i++)
	{
		if (modelChildrens[i] != NULL)
			modelChildrens[i]->SetMaterialNode(myMat);
	}
}

void Model::SetIsAlive(bool value)
{
	Entity::SetIsAlive(value);
	if (rootNode != NULL)
		rootNode->SetIsAlive(value);

	for (int i = 0; i < modelChildrens.size(); i++)
	{
		if (modelChildrens[i] != NULL)
			modelChildrens[i]->SetIsAlive(value);
	}
}

void Model::ChangeDrawState(Entity* nodeToChange, bool value)
{
	nodeToChange->SetIsAlive(value);
}

void Model::BindBuffer() {}

void Model::SetEnableDrawAABB(bool value)
{
}