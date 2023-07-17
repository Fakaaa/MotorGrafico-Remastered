#include "Primitive3D.h"
#include <iostream>
#include <glew.h>
#include <GLFW/glfw3.h>
#include "Material.h"

//INCLUIR ESTE PUNTO H EN CUALQUIER ENTIDAD QUE UTILICE AxisAlignedBoundingBox
#include "AxisAlignedBoundingBox.h"

Primitive3D::Primitive3D(Renderer* renderer) : Entity(renderer)
{
	_type = Cube;

	_path = "None Path";
	_useTexture = false;
	my_Mat = NULL;
	CreateDataModel();
	
	//Creamos el axisAlignedBoundingBox
	CreateMyAxisAlignedBoundingBox();

	//0. Atacheo mi internalData a mi AxisAlignedBoundingBox
	axisAlignedBoundingBox->AttachEntity(internalData, transform);

	vector<glm::vec3> _dataXYZ;

	switch (_type)
	{
	case Cube:
		// 1. OBTENER LA DATA XYZ EN LIMPIO
		_dataXYZ = FilterVertexDataXYZ(vertexCube, elementsForVertexCount, verticesCubeCount);
		// 2. PASAR LA DATA EN LIMPIO AL AxisAlignedBoundingBox 
		// = SetVerticesCollidersPositions(GenerateAxisAlignedBoundingBox(vector<glm::vec3> _values))
		axisAlignedBoundingBox->SetVerticesColliders(axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxPos(_dataXYZ),
			axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxCol());
		break;
	case Pyramid:
		// 1. OBTENER LA DATA XYZ EN LIMPIO
		_dataXYZ = FilterVertexDataXYZ(vertexPyramid, elementsForVertexCount, verticesPyramidCount);
		// 2. PASAR LA DATA EN LIMPIO AL AxisAlignedBoundingBox 
		// = SetVerticesCollidersPositions(GenerateAxisAlignedBoundingBox(vector<glm::vec3> _values))
		axisAlignedBoundingBox->SetVerticesColliders(axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxPos(_dataXYZ),
			axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxCol());
		break;
	}
}

Primitive3D::Primitive3D(Renderer * renderer, TypeModel typeModel) : Entity(renderer)
{
	_type = typeModel;

	_path = "None Path";
	_useTexture = false;
	my_Mat = NULL;
	CreateDataModel();

	//Creamos el axisAlignedBoundingBox
	CreateMyAxisAlignedBoundingBox();

	//0. Atacheo mi internalData a mi AxisAlignedBoundingBox
	axisAlignedBoundingBox->AttachEntity(internalData, transform);

	vector<glm::vec3> _dataXYZ;

	switch (_type)
	{
	case Cube:
		// 1. OBTENER LA DATA XYZ EN LIMPIO
		_dataXYZ = FilterVertexDataXYZ(vertexCube, elementsForVertexCount, verticesCubeCount);
		// 2. PASAR LA DATA EN LIMPIO AL AxisAlignedBoundingBox 
		// = SetVerticesCollidersPositions(GenerateAxisAlignedBoundingBox(vector<glm::vec3> _values))
		axisAlignedBoundingBox->SetVerticesColliders(axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxPos(_dataXYZ),
													 axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxCol());
		break;
	case Pyramid:
		// 1. OBTENER LA DATA XYZ EN LIMPIO
		_dataXYZ = FilterVertexDataXYZ(vertexPyramid, elementsForVertexCount, verticesPyramidCount);
		// 2. PASAR LA DATA EN LIMPIO AL AxisAlignedBoundingBox 
		// = SetVerticesCollidersPositions(GenerateAxisAlignedBoundingBox(vector<glm::vec3> _values))
		axisAlignedBoundingBox->SetVerticesColliders(axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxPos(_dataXYZ),
			axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxCol()); 

		break;
	}
}

Primitive3D::Primitive3D(Renderer* renderer, TypeModel typeModel, const char* filePath, bool useTransparency) : Entity(renderer)
{
	_type = typeModel;
	
	CreateDataModel();

	_path = filePath;
	_transparency = useTransparency;
	_useTexture = true;
	my_Mat = NULL;

	if (_transparency)
		BlendSprite();

	LoadTexture(_path, _transparency);

	//Creamos el axisAlignedBoundingBox
	CreateMyAxisAlignedBoundingBox();

	//0. Atacheo mi internalData a mi AxisAlignedBoundingBox
	axisAlignedBoundingBox->AttachEntity(internalData, transform);

	vector<glm::vec3> _dataXYZ;

	switch (_type)
	{
	case Cube:
		// 1. OBTENER LA DATA XYZ EN LIMPIO
		_dataXYZ = FilterVertexDataXYZ(vertexCube, elementsForVertexCount, verticesCubeCount);
		// 2. PASAR LA DATA EN LIMPIO AL AxisAlignedBoundingBox 
		// = SetVerticesCollidersPositions(GenerateAxisAlignedBoundingBox(vector<glm::vec3> _values))
		axisAlignedBoundingBox->SetVerticesColliders(axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxPos(_dataXYZ),
													 axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxCol());
		break;
	case Pyramid:
		// 1. OBTENER LA DATA XYZ EN LIMPIO
		_dataXYZ = FilterVertexDataXYZ(vertexPyramid, elementsForVertexCount, verticesPyramidCount);
		// 2. PASAR LA DATA EN LIMPIO AL AxisAlignedBoundingBox 
		// = SetVerticesCollidersPositions(GenerateAxisAlignedBoundingBox(vector<glm::vec3> _values))
		axisAlignedBoundingBox->SetVerticesColliders(axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxPos(_dataXYZ),
			axisAlignedBoundingBox->GenerateAxisAlignedBoundingBoxCol());
		break;
	}
}

Primitive3D::~Primitive3D(){}


string Primitive3D::GetClassName()
{
	return "Primitive3D";
}

void Primitive3D::UseMyMaterial()
{
	if (my_Mat != NULL)
		my_Mat->UseMaterial(renderer->GetCurrentShaderUse());
}

void Primitive3D::SetNewMaterial(Material * mat)
{
	my_Mat = mat;

	renderer->SetMaterial(my_Mat);
}

void Primitive3D::Draw(bool& wireFrameActive)
{
	if (isAlive) 
	{
		CheckIsModel();
		//----
		BindBuffer();

		if (_useTexture)
		{
			if (_transparency)
				BlendSprite();

			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _texture);

		}

		if (my_Mat != NULL)
			UseMyMaterial();

		//----
		switch (_type)
		{
		case Cube:
			axisAlignedBoundingBox->UpdateInternalDataBoundingBox(internalData, transform);

			renderer->Draw(indicesCubeCount, renderer->GetCurrentShaderUse(), internalData.localModel, wireFrameActive);

			// 3. LLAMAR AL DRAW DEL AxisAlignedBoundingBox en el draw de esta clase.
			axisAlignedBoundingBox->Draw(axisAlignedBoundingBox->GetEnableDraw());
			break;
		case Pyramid:
			axisAlignedBoundingBox->UpdateInternalDataBoundingBox(internalData, transform);

			renderer->Draw(indicesCubeCount, renderer->GetCurrentShaderUse(), internalData.localModel, wireFrameActive);
			
			// 3. LLAMAR AL DRAW DEL AxisAlignedBoundingBox en el draw de esta clase.
			axisAlignedBoundingBox->Draw(axisAlignedBoundingBox->GetEnableDraw());
			break;
		}

		if (_useTexture)
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);

			if (_transparency)
				UnBlendSprite();
		}
	}
	//----
}

void Primitive3D::LoadTexture(const char* path, bool transparent) {
	_useTexture = true;
	_transparency = transparent;
	texImporter.LoadTexture(path, data, _texture, _width, _height, _nrChannels, _transparency);
}

#pragma region FUNCIONES DE INICIALIZACION DE DATA

void Primitive3D::CreateDataModel()
{
	//---
	SetVAO();
	//---
	SetIBO();
	//---
	SetVBO();
	//---
	UnbindBuffers();
	//---
}

void Primitive3D::BindBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	BindGeneralData();
}

void Primitive3D::SetEnableDrawAABB(bool value)
{
	if (axisAlignedBoundingBox != NULL)
		axisAlignedBoundingBox->SetEnableDraw(value);
}

vector<glm::vec3> Primitive3D::FilterVertexDataXYZ(float * vertex, unsigned int countElementsForVertex, unsigned int sizeArrVertex)
{
	vector<vector<float>> vertices;
	vector<float> aux;

	vector<glm::vec3> dataReturn;

	int counter = 0;
	for (int j = 0; j < sizeArrVertex; j++) 
	{
		aux.push_back(vertex[j]);
		counter++;
		if (counter >= countElementsForVertex) 
		{
			counter = 0;
			vector<vector<float>>::iterator alreadyOnVertices = find(vertices.begin(), vertices.end(), aux);
			if (alreadyOnVertices == vertices.end())
			{
				vertices.push_back(aux);
				aux.clear();
			}
		}
	}

	aux.clear();

	for (int i = 0; i < vertices.size(); i++) 
	{
		vector<glm::vec3>::iterator alreadyOnData = find(dataReturn.begin(), dataReturn.end(), glm::vec3(vertices[i][0], vertices[i][1], vertices[i][2]));
		if(alreadyOnData == dataReturn.end())
			dataReturn.push_back(glm::vec3(vertices[i][0], vertices[i][1], vertices[i][2]));
	}

	return dataReturn;
}

void Primitive3D::BindGeneralData()
{
	glVertexAttribPointer(_positionLocation, 3, GL_FLOAT, GL_FALSE, elementsForVertexCount * sizeof(float), 0);
	glEnableVertexAttribArray(_positionLocation);
	glVertexAttribPointer(_colorLocation, 4, GL_FLOAT, GL_FALSE, elementsForVertexCount * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(_colorLocation);
	glVertexAttribPointer(_normalLocation, 3, GL_FLOAT, GL_FALSE, elementsForVertexCount * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(_normalLocation);
	glVertexAttribPointer(_texLocation, 2, GL_FLOAT, GL_FALSE, elementsForVertexCount * sizeof(float), (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(_texLocation);
}

void Primitive3D::SetVAO()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
}

void Primitive3D::SetVBO()
{
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	switch (_type)
	{
	case Cube:
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCube), vertexCube, GL_STATIC_DRAW);
		break;
	case Pyramid:
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPyramid), vertexPyramid, GL_STATIC_DRAW);
		break;
	}
	_positionLocation = glGetAttribLocation(renderer->GetCurrentShaderUse().getId(), "position");

	_colorLocation = glGetAttribLocation(renderer->GetCurrentShaderUse().getId(), "customColor");

	_normalLocation = glGetAttribLocation(renderer->GetCurrentShaderUse().getId(), "norm");

	_texLocation = glGetAttribLocation(renderer->GetCurrentShaderUse().getId(), "m_TexCoord");

	BindGeneralData();
}

void Primitive3D::SetIBO()
{
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	switch (_type)
	{
	case Cube:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexCube), indexCube, GL_STATIC_DRAW);
		break;
	case Pyramid:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexPyramid), indexPyramid, GL_STATIC_DRAW);
		break;
	}
}

void Primitive3D::UnbindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Primitive3D::BlendSprite()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Primitive3D::UnBlendSprite()
{
	glDisable(GL_BLEND);
}
#pragma endregion




