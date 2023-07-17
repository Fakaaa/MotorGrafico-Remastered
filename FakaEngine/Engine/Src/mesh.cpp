#include "Mesh.h"
#include "Material.h"
#include <glew.h>
#include <GLFW/glfw3.h>
#include "AxisAlignedBoundingBox.h"

Mesh::Mesh(Renderer* render) : Entity(render)
{
	_vao = 0;
	_vbo = 0;
	_ibo = 0;
	countIndices = 0;

	_uniformIsModelLocation = glGetUniformLocation(render->GetCurrentShaderUse().getId(), "model");
	_uniformProjectionLocation = glGetUniformLocation(render->GetCurrentShaderUse().getId(), "projection");
	_uniformViewLocation = glGetUniformLocation(render->GetCurrentShaderUse().getId(), "view");

	_positionLocation = glGetAttribLocation(render->GetCurrentShaderUse().getId(), "position");
	_texLocation = glGetAttribLocation(render->GetCurrentShaderUse().getId(), "m_TexCoord");
	_normalLocation = glGetAttribLocation(render->GetCurrentShaderUse().getId(), "norm");
	SetName("Mesh");
}

Mesh::~Mesh(){}

string Mesh::GetClassName()
{
	return "Mesh";
}

void Mesh::SetVBO(float* vertices, unsigned int numVertices)
{
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices, vertices, GL_STATIC_DRAW);
}

void Mesh::SetVAO()
{
	glGenBuffers(1, &_vao);
	glBindVertexArray(_vao);
}

void Mesh::SetIBO(unsigned int* indices, unsigned int numIndices)
{
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * numIndices, indices, GL_STATIC_DRAW);
}

void Mesh::BindDataMesh()
{
	glVertexAttribPointer(_positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * elementsForVertex,0);
	glEnableVertexAttribArray(_positionLocation);

	glVertexAttribPointer(_texLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * elementsForVertex,(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(_texLocation);

	glVertexAttribPointer(_normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * elementsForVertex,(void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(_normalLocation);
}

void Mesh::BindBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	BindDataMesh();
}

void Mesh::SetEnableDrawAABB(bool value)
{
	if(axisAlignedBoundingBox != NULL)
		axisAlignedBoundingBox->SetEnableDraw(value);
}

void Mesh::CreateMesh(float * vertices, unsigned int * indices, unsigned int numVertices, unsigned int numOfIndices)
{
	countIndices = numOfIndices;

	SetVAO();

	SetIBO(indices, numOfIndices);

	SetVBO(vertices, numVertices);

	BindDataMesh();

	CreateMyAxisAlignedBoundingBox();

	axisAlignedBoundingBox->AttachEntity(internalData, transform);

	axisAlignedBoundingBox->SetVerticesColliders(axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxPos(meshXYZVertices),
		axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxCol());

	UnbindBuffers();
}

void Mesh::Draw(bool& wireFrameActive)
{
	if (isAlive || InmortalObject) 
	{
		BindBuffer();

		GetAABB()->UpdateInternalDataBoundingBox(internalData, transform);

		renderer->Draw(countIndices, renderer->GetCurrentShaderUse(), internalData.localModel, wireFrameActive);
		
		GetAABB()->Draw(GetAABB()->GetEnableDraw());
	}
}

void Mesh::UnbindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}