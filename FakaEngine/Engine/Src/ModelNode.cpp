#include "ModelNode.h"

#include "ModelImporter.h"
#include "mesh.h"
#include "Texture.h"
#include "Material.h"

#include "AxisAlignedBoundingBox.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ModelNode::ModelNode(Renderer* render, aiNode* node) : Entity(render)
{
	_myNode = node;
	SetName(_myNode->mName.C_Str());
	allchildrensDone = false;

	CreateMyAxisAlignedBoundingBox();
	axisAlignedBoundingBox->AttachEntity(internalData, transform);
}

ModelNode::~ModelNode()
{
	for (int i = 0; i < _meshList.size(); i++)
	{
		if (_meshList[i] != NULL)
		{
			delete _meshList[i];
			_meshList[i] = NULL;
		}
	}

	if (axisAlignedBoundingBox != NULL)
	{
		delete axisAlignedBoundingBox;
		axisAlignedBoundingBox = NULL;
	}
}

string ModelNode::GetNodeName()
{
	return _myNode->mName.C_Str();
}

void ModelNode::PrintMyInfo()
{
	cout << "My Name is: " << GetName() << endl;
	//cout << "cout meshes: " << _meshList.size() << endl;
	//cout << "cout meshToTex: " << _meshToTex.size() << endl;
	cout << "Position X: " << transform.position.x << endl;
	cout << "Position Y: " << transform.position.y << endl;
	cout << "Position Z: " << transform.position.z << endl;
	cout << "---------------------------------------------" << endl;

}

void ModelNode::BindBuffer()
{
}

void ModelNode::SetEnableDrawAABB(bool value)
{
	axisAlignedBoundingBox->SetEnableDraw(value);
	for (int i = 0; i < _meshList.size(); i++)
	{
		if (_meshList[i]->GetAABB() != NULL) {
			_meshList[i]->GetAABB()->SetEnableDraw(value);
		}
	}
}

void ModelNode::SetupAxisAlignedBoundingBox()
{
	vector<glm::vec3> _dataXYZ;

	vector<Mesh*> allChildMeshes = GetChildNodesMeshes(this);

	for (int i = 0; i < allChildMeshes.size(); i++)
	{
		if (allChildMeshes[i] != NULL)
		{
			for (int j = 0; j < allChildMeshes[i]->meshXYZVertices.size(); j++)
			{
				_dataXYZ.push_back(allChildMeshes[i]->meshXYZVertices[j]);
			}
		}
	}

	if (_dataXYZ.size() <= 0 || allChildMeshes.size() <= 0)
	{
		if (axisAlignedBoundingBox != NULL)
		{
			delete axisAlignedBoundingBox;
			axisAlignedBoundingBox = NULL;
		}
	}
	else {
		axisAlignedBoundingBox->SetVerticesColliders(axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxPos(_dataXYZ),
			axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxCol());
	}
}

vector<Mesh*> ModelNode::GetChildNodesMeshes(ModelNode* node)
{
	vector<Mesh*> allChildMeshes;

	for (int i = 0; i < node->_meshList.size(); i++)
	{
		allChildMeshes.push_back(node->_meshList[i]);
	}

	for (int i = 0; i < node->GetChildrens().size(); i++)
	{
		Entity* entityChild = node->GetChildrens()[i];

		ModelNode* childNode = dynamic_cast<ModelNode*>(entityChild);

		if (childNode != NULL)
		{
			vector<Mesh*> currChildMeshes = GetChildNodesMeshes(childNode);

			for (int k = 0; k < currChildMeshes.size(); k++)
			{
				allChildMeshes.push_back(currChildMeshes[k]);
			}
		}
	}

	return allChildMeshes;
}

void ModelNode::Draw(bool& wireFrameActive)
{
	if (isAlive || InmortalObject)
	{
		if (axisAlignedBoundingBox != NULL)
		{
			axisAlignedBoundingBox->UpdateInternalDataBoundingBox(internalData, transform);
		}

		for (int i = 0; i < _meshList.size(); i++)
		{
			unsigned int materialIndex = _meshToTex[i];

			if (materialIndex < _textureList.size() && _textureList[materialIndex])
				_textureList[materialIndex]->BindTexture();

			if (myMat != NULL)
				myMat->UseMaterial(renderer->GetCurrentShaderUse());

			_meshList[i]->Draw(wireFrameActive);

			if (materialIndex < _textureList.size() && _textureList[materialIndex])
				_textureList[materialIndex]->UnbindTexture();
		}

		if (axisAlignedBoundingBox != NULL)
		{
			GetAABB()->Draw(GetAABB()->GetEnableDraw());
		}
	}
}

void ModelNode::SetMaterialNode(Material* mat)
{
	myMat = mat;
	renderer->SetMaterial(myMat);
}

void ModelNode::SetTexturesList(vector<Texture*>& textureList)
{
	_textureList = textureList;
}

void ModelNode::SetIsAlive(bool value)
{
	Entity::SetIsAlive(value);
	for (int i = 0; i < _meshList.size(); i++)
	{
		_meshList[i]->SetIsAlive(value);
	}
}