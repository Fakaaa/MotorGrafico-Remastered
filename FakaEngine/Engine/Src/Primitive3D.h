#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "PrivateClass/Export.h"
#include "Entity.h"
#include "TextureImporter.h"
#include <vector>
ENGINE_API enum TypeModel
{
	Cube,
	Pyramid
};

const int indicesCubeCount = 36;
const int verticesCubeCount = 288;
const int offsetNormalDataCube = 7;

const int indicesPyramidCount = 24;
const int verticesPyramidCount = 192;
const int offsetNormalDataPyramid = 7;

const int elementsForVertexCount = 12;

class ENGINE_API Primitive3D : public Entity
{
private:

	float vertexCube[verticesCubeCount] = {
	//	x		y		z		r		g		b		 a			 Normal           UV
	  -1.0,   -1.0,    1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f, 0.0f, 1.0f,  0.0f, 0.0f,	//0
	   1.0,   -1.0,    1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f, 0.0f, 1.0f,  1.0f, 0.0f,	//1
	   1.0,    1.0,    1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f, 0.0f, 1.0f,  1.0f, 1.0f,	//2
	  -1.0,    1.0,    1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f, 0.0f, 1.0f,  0.0f, 1.0f,	//3
	  -1.0,   -1.0,   -1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f, 0.0f,-1.0f,  0.0f, 0.0f,	//4
	   1.0,   -1.0,   -1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f, 0.0f,-1.0f,  1.0f, 0.0f,	//5
	   1.0,    1.0,   -1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f, 0.0f,-1.0f,  1.0f, 1.0f,	//6
	  -1.0,    1.0,   -1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f, 0.0f,-1.0f,  0.0f, 1.0f,	//7
																				   
	  -1.0,	   1.0,    1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f, 1.0f,	0.0f,  0.0f, 0.0f,	//8
	   1.0,	   1.0,    1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f, 1.0f,	0.0f,  1.0f, 0.0f,	//9
	   1.0,	  -1.0,    1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f,-1.0f,	0.0f,  1.0f, 1.0f,	//10
	  -1.0,	  -1.0,    1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f,-1.0f,	0.0f,  0.0f, 1.0f,	//11

	  -1.0,    1.0,    1.0,    1.0f,  1.0f,	  1.0f,    0.0f,   -1.0f, 0.0f,	0.0f,  1.0f, 1.0f,	//12
	   1.0,	   1.0,    1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	1.0f, 0.0f,	0.0f,  0.0f, 1.0f,	//13
	   1.0,	  -1.0,    1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	1.0f, 0.0f,	0.0f,  0.0f, 0.0f,	//14
	  -1.0,	  -1.0,    1.0,    1.0f,  1.0f,	  1.0f,    0.0f,   -1.0f, 0.0f,	0.0f,  1.0f, 0.0f,	//15

	  -1.0,   -1.0,   -1.0,    1.0f,  1.0f,	  1.0f,    0.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,	//16
	   1.0,   -1.0,   -1.0,    1.0f,  1.0f,	  1.0f,    0.0f,    1.0f, 0.0f, 0.0f,  1.0f, 0.0f,	//17
	   1.0,    1.0,   -1.0,    1.0f,  1.0f,	  1.0f,    0.0f,    1.0f, 0.0f, 0.0f,  1.0f, 1.0f,	//18
	  -1.0,    1.0,   -1.0,    1.0f,  1.0f,	  1.0f,    0.0f,   -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,	//19

	  -1.0,   -1.0,   -1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f,-1.0f, 0.0f,  0.0f, 0.0f,  //20
	   1.0,   -1.0,   -1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f,-1.0f, 0.0f,  1.0f, 0.0f,  //21
	   1.0,    1.0,   -1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f, 1.0f, 0.0f,  1.0f, 1.0f,  //22
	  -1.0,    1.0,   -1.0,    1.0f,  1.0f,	  1.0f,    0.0f,	0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  //23
	};
	float vertexPyramid[verticesPyramidCount] = {
		//	x		y		z		r		g		b		a		  Normal             UV 
		  -1.0f,  -1.0f,   0.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f, -0.5f, 0.0f, 0.5f,  0.0f, 0.0f, //0
		   0.0f,  -1.0f,   1.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f, -0.5f, 0.0f, 0.5f,	 1.0f, 0.0f, //1
		   1.0f,  -1.0f,   0.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f,  0.5f, 0.0f,-0.5f,	 0.0f, 0.0f, //2
		   0.0f,   1.0f,   0.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f, -0.5f, 0.0f, 0.5f,	 0.5f, 1.0f, //3
		   0.0f,  -1.0f,  -1.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f,  0.5f, 0.0f,-0.5f,	 1.0f, 0.0f,//4

		   0.0f,   1.0f,   0.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f, -0.5f, 0.0f,-0.5f,	 0.5f, 1.0f, //5
		   0.0f,   1.0f,   0.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f,  0.5f, 0.0f,-0.5f,	 0.5f, 1.0f, //6
		   0.0f,   1.0f,   0.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f,  0.5f, 0.0f, 0.5f,	 0.5f, 1.0f, //7

		  -1.0f,  -1.0f,   0.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f,  0.0f,-1.0f, 0.0f,  1.0f, 0.0f, //8
		   0.0f,  -1.0f,   1.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f,  0.0f,-1.0f, 0.0f,	 1.0f, 1.0f, //9
		   1.0f,  -1.0f,   0.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f,  0.0f,-1.0f, 0.0f,	 0.0f, 1.0f, //10
		   0.0f,  -1.0f,  -1.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f,  0.0f,-1.0f, 0.0f,	 0.0f, 0.0f, //11

		 - 1.0f,  -1.0f,   0.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f, -0.5f, 0.0f,-0.5f,  0.0f, 0.0f, //12
		   0.0f,  -1.0f,   1.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f,  0.5f, 0.0f, 0.5f,	 1.0f, 0.0f, //13
		   1.0f,  -1.0f,   0.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f,  0.5f, 0.0f, 0.5f,	 0.0f, 0.0f, //14
		   0.0f,  -1.0f,  -1.0f,  1.0f,	  1.0f,	  1.0f,	   1.0f, -0.5f, 0.0f,-0.5f,	 1.0f, 0.0f //15
	};																				
	unsigned int indexCube[indicesCubeCount] = {	
		0, 1, 2, 2, 3, 0,	   //Frente
		14, 17, 18, 18, 13, 14,//Derecha
		4, 7, 6, 6, 5, 4,      //Atras			DONE
		16, 15, 12, 12, 19, 16,//Izquierda
		20, 11, 10, 10, 21, 20,//Abajo
		8, 9, 22, 22, 23, 8    //Arriba
	};
	unsigned int indexPyramid[indicesPyramidCount] = {
		0,3,1,13,7,14,
		12,5,15, 4,6,2,
		11,8,9, 9,10,11
	};
	TypeModel _type;

	//Texture Importer
	bool _useTexture = true;
	unsigned char* data;
	int _height;
	int _width;
	int _nrChannels;
	const char* _path;
	bool _transparency;
	unsigned int _texture;
	TextureImporter texImporter;

	Material* my_Mat;

	vector<glm::vec3> FilterVertexDataXYZ(float * vertex, unsigned int countElementsForVerte, unsigned int sizeArrVertex);
protected:
	void BindGeneralData();
	void SetVAO();
	void SetVBO();
	void SetIBO();
	void UnbindBuffers();
	void CreateDataModel();
	void BlendSprite();
	void UnBlendSprite();
	void BindBuffer() override;
	void SetEnableDrawAABB(bool value) override;

public:
	Primitive3D(Renderer* renderer);
	Primitive3D(Renderer* renderer, TypeModel typeModel);
	Primitive3D(Renderer* renderer, TypeModel typeModel, const char* filePath, bool useTransparency);
	~Primitive3D();
	//GETERS
	TypeModel GetTypeModel() { return _type; }
	//FUNCTIONS
	string GetClassName() override;
	void UseMyMaterial();
	void SetNewMaterial(Material* mat);
	void LoadTexture(const char* path, bool transparent);
	void SetUseTexture(bool useTexture) { _useTexture = useTexture; }
	void Draw(bool& wireFrameActive) override;
	
};
#endif // !MODEL_H