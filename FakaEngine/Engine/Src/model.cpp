#include "Model.h"

#include <glew.h>
#include <GLFW/glfw3.h>

#include "ModelImporter.h"
#include "PlaneBSP.h"

#include "ModelNode.h"
#include "Utils/Utils.h"

#include "AxisAlignedBoundingBox.h"

#include "Camera.h"

Model::Model(Renderer* render, bool hasBPSPlane, Entity* parentBSP_Planes) : Entity(render)
{
	myMat = NULL;
	rootNode = NULL;
	modelImporter = new ModelImporter();

	hasBSPPlanes = hasBPSPlane;
	this->parentBSP_Planes = parentBSP_Planes;
	modelImporter->SetBSP_PlanesParent(parentBSP_Planes);

	CreateMyAxisAlignedBoundingBox();
	axisAlignedBoundingBox->AttachEntity(internalData, transform);
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

	if (rootNode != NULL) {
		AddChildren(rootNode);
		rootNode->_textureList = textureList;
	}

	for (int i = 0; i < modelChildrens.size(); i++)
	{
		if (modelChildrens[i] != NULL)
			modelChildrens[i]->_textureList = textureList;
	}

	vector<glm::vec3> _dataXYZ;

	for (int i = 0; i < modelMeshes.size(); i++)
	{
		for (int j = 0; j < modelMeshes[i]->meshXYZVertices.size(); j++)
		{
			if (!Utils::CheckStringCoincidence(modelMeshes[i]->GetMeshName(), "BSP_Plane"))
			{
				_dataXYZ.push_back(modelMeshes[i]->meshXYZVertices[j]);
			}
		}
	}

	axisAlignedBoundingBox->SetVerticesColliders(axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxPos(_dataXYZ),
		axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxCol());
}

void Model::Draw(bool& wireFrameActive)
{
	if (isAlive || InmortalObject)
	{
		axisAlignedBoundingBox->UpdateInternalDataBoundingBox(internalData, transform);

		if (rootNode != NULL)
			rootNode->Draw(wireFrameActive);

		for (int i = 0; i < modelChildrens.size(); i++)
		{
			if (modelChildrens[i] != NULL)
				modelChildrens[i]->Draw(wireFrameActive);
		}

		axisAlignedBoundingBox->Draw(axisAlignedBoundingBox->GetEnableDraw());
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
	if (axisAlignedBoundingBox != NULL)
		axisAlignedBoundingBox->SetEnableDraw(value);
}

vector<PlaneBSP*> Model::GetBSPs()
{
	return modelImporter->GetBSP_PlanesGenerated();
}