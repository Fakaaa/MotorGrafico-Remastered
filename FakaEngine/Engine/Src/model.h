#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

//#include "PlaneBSP.h"

using namespace std;

class ModelImporter;
class ModelNode;

#define BSP_PLANE1 0
#define BSP_PLANE2 1
#define BSP_PLANE3 2

class ENGINE_API Model : public Entity
{
private:
	ModelImporter* modelImporter = NULL;
public:
	Model(Renderer* render, bool hasBPSPlane);
	~Model();

	//PlaneBSP* planeBSP1;
	//PlaneBSP* planeBSP2;
	//PlaneBSP* planeBSP3;

	bool hasBSPPlanes;

	void LoadModel(const string& filePath, const string& texturePath);
	void Draw(bool& wireFrameActive) override;
	void UnloadModel();

	string GetNameOfClass() override;

	void SetMaterial(Material* mat);
	virtual void SetIsAlive(bool value);
	void ChangeDrawState(Entity* nodeToChange, bool value);

	ModelNode* GetMyModelNode() { return rootNode; }
	vector<ModelNode*> GetModelChildrens() { return modelChildrens; }
	vector<Mesh*> GetModelMeshes() { return modelMeshes; }

	void updateNodesIndexBSP();
	void updateBSPPlanes(glm::vec3 posPlane1, glm::vec3 posPlane2, glm::vec3 posPlane3);
	void checkBSPRecursive(Camera* camera, Entity* rootNode);
	bool checkIfIsOnSide(Camera* camera, Entity* node);
protected:
	void BindBuffer() override;
	void SetEnableDrawAABB(bool value) override;
private:
	vector<Texture*> textureList;
	vector<ModelNode*> modelChildrens;
	vector<Mesh*> modelMeshes;
	ModelNode* rootNode;
	Material* myMat;
};

#endif
