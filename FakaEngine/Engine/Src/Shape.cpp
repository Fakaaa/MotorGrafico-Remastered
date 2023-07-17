#include <iostream>
#include "Shape.h"
#include <glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "AxisAlignedBoundingBox.h"

const int countElementsForVertex = 5;

const int countElementsBufferTri = 15;
const int countElementsIndexBufferTri = 4;

const int countElementsBufferQuad = 20;
const int countElementsIndexBufferQuad = 6;

float vertexBufferTri[countElementsBufferTri] = {
	//X		  Y		 Z     U	 V
	-0.5f , -0.5f , 0.0f, 0.0f, 0.0f,
	 0.5f , -0.5f , 0.0f, 1.0f, 0.0f,
	 0.0f ,  0.5f , 0.0f, 0.5f, 1.0f,
};

float vertexBufferQuad[countElementsBufferQuad] = {
	//X		  Y		 Z		U     V
	-0.5f , -0.5f , 0.0f,  0.0f, 0.0f,
	 0.5f , -0.5f , 0.0f,  1.0f, 0.0f,
	 0.5f ,  0.5f , 0.0f,  1.0f, 1.0f,
	-0.5f ,  0.5f , 0.0f,  0.0f, 1.0f,
};

unsigned int indexBufferTri[countElementsIndexBufferTri] =
{
	0,1,2,0,
};

unsigned int indexBufferQuad[countElementsIndexBufferQuad] =
{
	0, 1, 2, 2, 3, 0,
};

Shape::Shape(Renderer * _renderer,TypeShape typeShape, const char * filePath): Entity2D(_renderer)
{
	_currentShape = typeShape;
	renderer = _renderer;
	_path = filePath;
	_transparency = false;

	material = NULL;
	_typeMaterial = TypeMaterial::TextureType;


	texImporter = new Texture(filePath, _transparency);

	if (_transparency)
		texImporter->BlendTexture();

	texImporter->LoadTexture(_path, _transparency);
	
	CreateDataShape();
}

Shape::Shape(Renderer * _renderer, TypeShape typeShape) : Entity2D(_renderer)
{
	_currentShape = typeShape;
	renderer = _renderer;
	_typeMaterial = TypeMaterial::ColorType;
	_path = "None Path";
	CreateDataShape();

}
Shape::~Shape()
{
	UnbindBuffers();
	if (texImporter != NULL) 
	{
		delete texImporter;
		texImporter = NULL;
	}
}
void Shape::SetNewMaterial(Material * mat)
{
	material = mat;

	renderer->SetMaterial(material);
}

void Shape::Draw(bool& wireFrameActive) 
{
	if (isAlive) {
		BindBuffer();
		switch (_currentShape)
		{
		case TypeShape::QUAD:
			UseShape(countElementsIndexBufferQuad, renderer->GetCurrentShaderUse(), wireFrameActive);
			break;
		case TypeShape::TRIANGLE:
			UseShape(countElementsIndexBufferTri, renderer->GetCurrentShaderUse(), wireFrameActive);
			break;
		}
	}
}

string Shape::GetClassName()
{
	return "Shape";
}

void Shape::UseShape(int indices, Shader& shader, bool& wireFrameActive)
{
	if (renderer != NULL)
	{
		CheckIsModel();

		if (_typeMaterial == TypeMaterial::TextureType)
		{
			texImporter->BindTexture();
		}
		
		if(material != NULL)
			material->UseMaterial(shader);
		
		renderer->Draw(indices, shader, internalData.localModel, wireFrameActive);

		if (_typeMaterial == TypeMaterial::TextureType) 
		{
			texImporter->UnbindTexture();
		}
	}
}

void Shape::LoadTexture(const char* filePath) 
{
	texImporter->LoadTexture(filePath, false);
}
//==============================================

unsigned int Shape::GetVbo(){
	return _vbo;
}

#pragma region FUNCIONES DE INICIALIZACION DE DATA

void Shape::CreateDataShape()
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

void Shape::BindBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	BindGeneralData();
}

void Shape::SetEnableDrawAABB(bool value)
{
	if (axisAlignedBoundingBox != NULL)
		axisAlignedBoundingBox->SetEnableDraw(value);
}

void Shape::BindGeneralData()
{
	glVertexAttribPointer(_positionLocation, 3, GL_FLOAT, GL_FALSE, countElementsForVertex * sizeof(float), 0);
	glEnableVertexAttribArray(_positionLocation);
	glVertexAttribPointer(_texLocation, 2, GL_FLOAT, GL_FALSE, countElementsForVertex * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(_texLocation);
}

void Shape::SetVAO()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
}

void Shape::SetIBO()
{
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	switch (_currentShape)
	{
	case TypeShape::QUAD:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBufferQuad), indexBufferQuad, GL_STATIC_DRAW);
		break;
	case TypeShape::TRIANGLE:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBufferTri), indexBufferTri, GL_STATIC_DRAW);
		break;
	}
}

void Shape::SetVBO()
{
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	switch (_currentShape)
	{
	case TypeShape::QUAD:
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferQuad), vertexBufferQuad, GL_DYNAMIC_DRAW);
		break;
	case TypeShape::TRIANGLE:
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferTri), vertexBufferTri, GL_DYNAMIC_DRAW);
		break;
	}

	_positionLocation = glGetAttribLocation(renderer->GetCurrentShaderUse().getId(), "position");
	if (texImporter != NULL) 
	{
		glUniform1i(_texLocation = glGetUniformLocation(renderer->GetCurrentShaderUse().getId(), "ourTexture"), texImporter->GetTexture());
		cout << "ENTRE: "+ texImporter->GetTexture() << endl;
	}
	BindGeneralData();
}

void Shape::UnbindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

#pragma endregion


