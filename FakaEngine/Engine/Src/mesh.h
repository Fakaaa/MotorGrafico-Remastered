#ifndef MESH_H
#define MESH_H

#include "Entity.h"
#include <string>
#include <vector>

const int elementsForVertex = 8;

class ENGINE_API Mesh : public Entity
{
private:	
	int countIndices;

protected:
	void SetVBO(float* vertices, unsigned int numVertices);
	void SetVAO();
	void SetIBO(unsigned int* indices, unsigned int numIndices);

	void BindDataMesh();
	void UnbindBuffers();

	void BindBuffer() override;
	void SetEnableDrawAABB(bool value) override;
public:
	Mesh(Renderer* render);
	~Mesh();

	string GetClassName() override;
	vector<glm::vec3> meshXYZVertices;

	void CreateMesh(float* vertices, unsigned int* indices, unsigned int numVertices, unsigned int numOfIndices);
	void Draw(bool& wireFrameActive) override;
};
#endif
