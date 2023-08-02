#include "ModelImporter.h"

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

#include "ModelNode.h"
#include "PlaneBSP.h"
#include "Utils/Utils.h"

#include <glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ModelImporter::ModelImporter()
{
	indexChildrenLoad = 0;
}

ModelImporter::~ModelImporter() {}

ModelNode* ModelImporter::LoadModel(vector<Mesh*>& modelMeshes, const string& filePath, const string& texturePath, ModelNode* rootNode, vector<ModelNode*>& childrens, vector<Texture*>& textureList, Renderer* render)
{
	ClearNodesOldModel();
	ClearAuxiliarNodesOldModel();

	Assimp::Importer imporeter;
	const aiScene* scene = imporeter.ReadFile(filePath,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene)
	{
		cout << "ERROR IMPORTER: Model failed to load, Location:" << filePath << " " << imporeter.GetErrorString() << endl;
		return NULL;
	}

	rootNode = new ModelNode(render, scene->mRootNode);
	auxiliarNodes.push(rootNode);
	LoadNode(scene->mRootNode, scene, childrens, render);

	LoadMesh(modelMeshes, rootNode, scene, render);
	LoadMesh(modelMeshes, childrens, scene, render);

	LoadMaterial(scene, texturePath, textureList);

	GenerateBSP_Planes(rootNode, childrens);

	return rootNode;
}

void ModelImporter::LoadNode(aiNode* node, const aiScene* scene, vector<ModelNode*>& childrens, Renderer* render)
{
	int i = 0;
	if (node != scene->mRootNode)
	{
		nodes.push_back(node);
	}
	for (i = 0; i < node->mNumChildren; i++)
	{
		ModelNode* n = new ModelNode(render, node->mChildren[i]);

		childrens.push_back(n);

		if (auxiliarNodes.size() > 0) {
			if (!auxiliarNodes.top()->allchildrensDone) {
				auxiliarNodes.top()->AddChildren(n);
				auxiliarNodes.top()->SetScale(1, 1, 1);
			}

			if (i >= node->mNumChildren - 1)
			{
				auxiliarNodes.top()->allchildrensDone = true;
			}
		}

		if (node->mChildren[i]->mNumChildren > 0) {
			auxiliarNodes.push(n);
		}
		LoadNode(node->mChildren[i], scene, childrens, render);
	}

	while (node != scene->mRootNode && !auxiliarNodes.empty() && auxiliarNodes.top()->allchildrensDone) {
		auxiliarNodes.pop();
	}
}

void ModelImporter::LoadMesh(vector<Mesh*>& modelMeshes, vector<ModelNode*> childrens, const aiScene* scene, Renderer* render)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = 0; j < nodes[i]->mNumMeshes; j++)
		{
			LoadMesh(modelMeshes, scene->mMeshes[nodes[i]->mMeshes[j]], scene, childrens[i], render);
		}
	}
}

void ModelImporter::LoadMesh(vector<Mesh*>& modelMeshes, ModelNode* rootNode, const aiScene* scene, Renderer* render)
{
	for (int j = 0; j < scene->mRootNode->mNumMeshes; j++)
	{
		LoadMesh(modelMeshes, scene->mMeshes[scene->mRootNode->mMeshes[j]], scene, rootNode, render);
	}
}

void ModelImporter::LoadMesh(vector<Mesh*>& modelMeshes, aiMesh* mesh, const aiScene* scene, ModelNode*& nodeMesh, Renderer* render)
{
	Mesh* newMesh = new Mesh(render);
	vector<float> vertices;
	vector<unsigned int> indices;

	vector<float> valuesX;
	vector<float> valuesY;
	vector<float> valuesZ;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z });
		newMesh->meshXYZVertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f,  0.0f });
		}
		vertices.insert(vertices.end(), { mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z });

		//We save the positions of the mesh when detect a coincidence with the key for bsp planes.
		if (Utils::CheckStringCoincidence(nodeMesh->GetName(), bspPlaneKey.c_str()))
		{
			valuesX.push_back(mesh->mVertices[i].x);
			valuesY.push_back(mesh->mVertices[i].y);
			valuesZ.push_back(mesh->mVertices[i].z);
		}
	}

	if (Utils::CheckStringCoincidence(nodeMesh->GetName(), bspPlaneKey.c_str()))
	{
		newMesh->SetInmortalObject(true);

		//We create the vertices struct and push this to a vector for compute later.
		planesPosition.push_back(PlaneBSP::VerticesBSP(Utils::FindMaxValue(valuesX), Utils::FindMinValue(valuesX),
			Utils::FindMaxValue(valuesY), Utils::FindMinValue(valuesY), Utils::FindMaxValue(valuesZ), Utils::FindMinValue(valuesZ)));
	}

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	newMesh->SetMeshName(nodeMesh->GetName());

	nodeMesh->_meshList.push_back(newMesh);
	nodeMesh->_meshToTex.push_back(mesh->mMaterialIndex);
	nodeMesh->AddChildren(newMesh);

	modelMeshes.push_back(newMesh);
}

void ModelImporter::LoadMaterial(const aiScene* scene, const string& texturePath, vector<Texture*>& textureList)
{
	textureList.resize(scene->mNumMaterials);

	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int index = string(path.data).rfind("\\");
				string fileName = string(path.data).substr(index + 1);

				string texPath = texturePath + fileName;

				cout << "Texture name:" << texPath << endl;

				textureList[i] = new Texture(texPath.c_str(), false);

				if (!textureList[i]->LoadTexture(false))
				{
					cout << "Failed to load texture! " << texPath << endl;
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}
	}
}

void ModelImporter::ClearNodesOldModel()
{
	nodes.clear();
}

void ModelImporter::ClearAuxiliarNodesOldModel()
{
	//Vaciar el stack del modelo anterior
	while (!auxiliarNodes.empty())
	{
		auxiliarNodes.pop();
	}
}

void ModelImporter::SetBSP_PlanesParent(Entity* newParent)
{
	bspsParent = newParent;
}

vector<PlaneBSP*> ModelImporter::GetBSP_PlanesGenerated()
{
	return resultBSPs;
}

void ModelImporter::LoadTextureFromFile(aiTextureType type)
{
}

void ModelImporter::GenerateBSP_Planes(ModelNode* rootNode, vector<ModelNode*> childrens)
{
	int currentIndex = 0;
	string rootSceneName = rootNode->GetNodeName();

	if (Utils::CheckStringCoincidence(rootSceneName, bspPlaneKey))
	{
		PlaneBSP* newBsp = new PlaneBSP();

		newBsp->SetName(rootSceneName);
		newBsp->SetVerticesBSP(planesPosition[currentIndex]);

		currentIndex++;

		resultBSPs.push_back(newBsp);
	}
	for (int i = 0; i < childrens.size(); i++)
	{
		string currNodeName = childrens[i]->GetNodeName();
		if (Utils::CheckStringCoincidence(currNodeName, bspPlaneKey))
		{
			PlaneBSP* newBsp = new PlaneBSP();

			newBsp->SetName(currNodeName);
			newBsp->SetVerticesBSP(planesPosition[currentIndex]);
			currentIndex++;

			cout << "Pushed a new BSP_Plane with KEY: " << currNodeName << endl;

			resultBSPs.push_back(newBsp);
		}
	}
}