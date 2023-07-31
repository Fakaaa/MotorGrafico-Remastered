#ifndef SHAPE_H
#define SHAPE_H
#include "PrivateClass/Export.h"

#include "Entity2D.h"
#include "../Lib/glm/vec4.hpp"
#include "Texture.h"

static enum TypeShape
{
	TRIANGLE = 0x0004,
	QUAD = 0x0007,
};
static enum TypeMaterial
{
	TextureType = 1,
	ColorType = 2,
};
//typedef unsigned int GLenum;
class ENGINE_API Shape : public Entity2D
{
private:
	float* _vertexBuffer;
	unsigned int _vbo;
	unsigned int _currentShape;
	TypeMaterial _typeMaterial;

	//Texture
	unsigned char* data;
	int _height;
	int _width;
	int _nrChannels;
	const char* _path;
	bool _transparency;
	Texture* texImporter = NULL;

	//Texture
	//unsigned int texture;
	//const char* _filePathTexture;
	//TextureImporter textureImporter;
	//unsigned char* data;
	//int width;
	//int height;
	//int nrChannels;
protected:
	void BindBuffer() override;
	void SetEnableDrawAABB(bool value) override;
	void SetVAO();
	void SetVBO();
	void SetIBO();
	void UnbindBuffers();
	void CreateDataShape();
	void UseShape(int indices, Shader& shader, bool& wireFrameActive);
public:
	void LoadTexture(const char* filePath);

	void Draw(bool& wireFrameActive) override;

	string GetNameOfClass() override;

	unsigned int _texture;

	Shape(Renderer* _renderer, TypeShape typeShape, const char* filePath);
	Shape(Renderer* _renderer, TypeShape typeShape);
	~Shape();

	//Textures

	void SetNewMaterial(Material* mat);

	void BindGeneralData();
	unsigned int GetVbo();
	unsigned int GetCurrentShape() { return _currentShape; }
};
#endif