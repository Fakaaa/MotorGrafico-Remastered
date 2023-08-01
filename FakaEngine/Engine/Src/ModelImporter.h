#ifndef MODELIMPORTER_H
#define MODELIMPORTER_H

#include <iostream>
#include <stack>
#include <vector>
#include <string>

#include "PrivateClass/Export.h"

#include "glm/vec3.hpp"

struct aiNode;
struct aiMesh;
struct aiScene;

enum aiTextureType;

class ModelNode;
class Texture;
class Renderer;
class Mesh;

using namespace std;

class ENGINE_API ModelImporter
{
private:
	int indexChildrenLoad;

	int amountForward;
	//ModelNode* auxNode;
	stack<ModelNode*> auxiliarNodes;
	vector<aiNode*> nodes;

	vector<glm::vec3> planesPosition;
	vector<vector<glm::vec3>> planesBSP;

	void LoadNode(aiNode* node, const aiScene* scene, vector<ModelNode*>& childrens, Renderer* render);
	void LoadMesh(vector<Mesh*>& modelMeshes, vector<ModelNode*> childrens, const aiScene* scene, Renderer* render);
	void LoadMesh(vector<Mesh*>& modelMeshes, ModelNode* rootNode, const aiScene* scene, Renderer* render);
	void LoadMesh(vector<Mesh*>& modelMeshes, aiMesh* mesh, const aiScene* scene, ModelNode*& nodeMesh, Renderer* render);
	void LoadMaterial(const aiScene* scene, const string& texturePath, vector<Texture*>& textureList);
	void LoadTextureFromFile(aiTextureType type);
public:
	ModelImporter();
	~ModelImporter();
	ModelNode* LoadModel(vector<Mesh*>& modelMeshes, const string& filePath, const string& texturePath, ModelNode* rootNode, vector<ModelNode*>& childrens, vector<Texture*>& textureList, Renderer* render);
	void ClearNodesOldModel();
	void ClearAuxiliarNodesOldModel();
	vector<vector<glm::vec3>> getPlanesBSP() { return planesBSP; }
};

#endif
