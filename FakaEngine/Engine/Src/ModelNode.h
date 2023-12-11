#ifndef MODELNODE_H
#define MODELNODE_H

#include <vector>

#include "Entity.h"
#include "PrivateClass/Export.h"

struct aiNode;
struct aiMesh;
struct aiScene;

class Mesh;
class Texture;


class ENGINE_API ModelNode : public Entity
{
public:
	ModelNode(Renderer* render, aiNode* node);
	~ModelNode();

	void Draw(bool& wireFrameActive) override;

	void SetMaterialNode(Material* mat);
	void SetTexturesList(vector<Texture*>& textureList);
	virtual void SetIsAlive(bool value);
	vector<Mesh*> _meshList;
	vector<Texture*> _textureList;
	vector<unsigned int> _meshToTex;

	aiNode* GetAssimpNode() { return _myNode; };
	string GetNodeName();
	bool allchildrensDone;

	void PrintMyInfo();
	void SetEnableDrawAABB(bool value) override;
	void SetIndexBSPPlanes(int plane1, int plane2, int plane3);
	void SetupAxisAlignedBoundingBox();
	void ClearIndexBSPPlanes();

	vector<Mesh*> GetChildNodesMeshes(ModelNode* node);
protected:
	void BindBuffer() override;
private:
	Material* myMat;
	aiNode* _myNode;
};
#endif // !MODELNODE_H