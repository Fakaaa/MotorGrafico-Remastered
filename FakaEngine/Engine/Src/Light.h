#ifndef LIGHT_H
#define LIGHT_H
#include "Entity.h"

const int lightIndicesCubeCount = 36;
const int lightCubeCount = 48;

class ENGINE_API Light : public Entity
{
public:

	static int nr_of_directional_light;
	static int nr_of_point_light;
	static int nr_of_spot_light;

	int my_Id;

	enum TypeLight
	{
		Point,
		Directional,
		Spot
	};

	Light(Renderer* _render, TypeLight type, Camera* _camera);
	Light(glm::vec3 colour, glm::vec3 ambient, glm::vec3 diffuse
		, glm::vec3 specular, Renderer* render, TypeLight type, Camera* _camera);

	void UseLight();

	string GetNameOfClass() override;

	void SetColorLight(float r, float g, float b);
	glm::vec3 GetColorLight();

	void SetTypeLightDefault(TypeLight type);
	void SetPointLightCustom(float linearVal, float quadraticVal, float cutOffValue);
	void SetSpotLightCustom(float linearVal, float quadraticVal, float cutOffValue, float outerCutOffValue);
	void SetDirectionalLightCustom(glm::vec3 direction);

	TypeLight GetTypeLight() { return _typeLight; }
	string GetTypeLightString();

	void SetIdLight(int id) { my_Id = id; }
	int GetMyId() { return my_Id; }

	//=====================================================
	//DIRECTIONAL LIGHT
	void SetUniformDirectionLightLocation(Shader& shader, int iter);
	unsigned int GetUniformDirectionalLight() { return _uniformDirectionLocation; }

	void SetUniformAmbDirectionalLoc(Shader& shader, int iter);
	unsigned int GetUniformAmbDirectionalLoc() { return _uniformAmbDirectionalLoc; }

	void SetUniformDiffDirectionalLoc(Shader& shader, int iter);
	unsigned int GetUniformDiffDirectionalLoc() { return _uniformDiffDirectionalLoc; }

	void SetUniformSpecDirectionalLoc(Shader& shader, int iter);
	unsigned int GetUniformSpecDirectionalLoc() { return _uniformSpecDirectionalLoc; }

	void SetUniformColourDirectional(Shader& shader, int iter);
	unsigned int GetUniformColourDirectional() { return _unifromColourDirectional; }
	//=====================================================
	//POINT LIGHT
	void SetUniformAmbientPointLoc(Shader& shader, int iter);
	unsigned int GetUniformAmbPointLoc() { return _uniformAmbientPointLoc; }

	void SetUniformDiffPointLoc(Shader& shader, int iter);
	unsigned int GetUniformDiffPointLoc() { return _uniformDiffusePointLoc; }

	void SetUniformSpecPointLoc(Shader& shader, int iter);
	unsigned int GetUniformSpecPointLoc() { return _uniformSpecularPointLoc; }

	void SetUniformLinearPointLight(Shader& shader, int iter);
	unsigned int GetUniformLinearPointLight() { return _uniformLinearPointLight; }

	void SetUniformQuadraticPointLight(Shader& shader, int iter);
	unsigned int GetUniformQuadraticPointLight() { return _uniformQuadraticPointLight; }

	void SetUniformConstPointLight(Shader& shader, int iter);
	unsigned int GetUniformConstPointLight() { return _uniformConstPointLight; }

	void SetUniformPosLightPoint(Shader& shader, int iter);
	unsigned int GetUniformPosLightPoint() { return _uniformPosLightPointLoc; }

	void SetUniformColourPoint(Shader& shader, int iter);
	unsigned int GetUniformColourPoint() { return _unifromColourPoint; }

	//=====================================================
	//SPOT LIGHT
	void SetUniformAmbientSpotLoc(Shader& shader, int iter);
	unsigned int GetUniformAmbSpotLoc() { return _uniformAmbientSpotLoc; }

	void SetUniformDiffSpotLoc(Shader& shader, int iter);
	unsigned int GetUniformDiffSpotLoc() { return _uniformDiffuseSpotLoc; }

	void SetUniformSpecSpotLoc(Shader& shader, int iter);
	unsigned int GetUniformSpecSpotLoc() { return _uniformSpecularSpotLoc; }

	void SetUniformLinearSpotLight(Shader& shader, int iter);
	unsigned int GetUniformLinearSpotLight() { return _uniformLinearSpotLight; }

	void SetUniformQuadraticSpotLight(Shader& shader, int iter);
	unsigned int GetUniformQuadraticSpotLight() { return _uniformQuadraticSpotLight; }

	void SetUniformConstSpotLight(Shader& shader, int iter);
	unsigned int GetUniformConstSpotLight() { return _uniformConstSpotLight; }

	void SetUniformCutOffSopttLight(Shader& shader, int iter);
	unsigned int GetUniformCutOffSopttLight() { return _uniformCutOffSpotLight; }

	void SetUniformOuterCutOffSopttLight(Shader& shader, int iter);
	unsigned int GetUniformOuterCutOffSopttLight() { return _uniformOuterCutOffSpotLight; }

	void SetUniformPosLightSpot(Shader& shader, int iter);
	unsigned int GetUniformPosLightSpot() { return _uniformPosLightSpotLoc; }

	void SetUniformSpotLightDirection(Shader& shader, int iter);
	unsigned int GetUniformSpotLightDirection() { return _uniformSpotLightDirection; }

	void SetUniformColourSpot(Shader& shader, int iter);
	unsigned int GetUniformColourSpot() { return _unifromColourSpot; }
	//=======================================================
	//CAMERA POS
	void SetUniformPosCameraLocation(Shader& shader);
	unsigned int GetUniformPosCameraLocation() { return _uniformPosCameraLocation; }

	//==============================

	void SetColour(glm::vec3 colour) { _colour = colour; }
	void SetColour(float v1, float v2, float v3)
	{
		_colour[0] = v1;
		_colour[1] = v2;
		_colour[2] = v3;
	}
	void SetAmbient(glm::vec3 ambient) { _ambient = ambient; }
	void SetAmbient(float v1, float v2, float v3)
	{
		_ambient[0] = v1;
		_ambient[1] = v2;
		_ambient[2] = v3;
	}
	void SetDiffuse(glm::vec3 diffuse) { _diffuse = diffuse; }
	void SetDiffuse(float v1, float v2, float v3)
	{
		_diffuse[0] = v1;
		_diffuse[1] = v2;
		_diffuse[2] = v3;
	}
	void SetSpecular(glm::vec3 specular) { _specular = specular; }
	void SetSpecular(float v1, float v2, float v3)
	{
		_specular[0] = v1;
		_specular[1] = v2;
		_specular[2] = v3;
	}

	glm::vec3 GetColour() { return _colour; }
	glm::vec3 GetAmbient() { return _ambient; }
	glm::vec3 GetDiffuse() { return _diffuse; }
	glm::vec3 GetSpecular() { return _specular; }

	void SetTypeLightPoint(float linearVal, float quadraticVal, float cutOffValue);
	void SetTypeLightSpot(float linearVal, float quadraticVal, float cutOffValue, float outerCutOffValue);
	void SetTypeLightDirectional(glm::vec3 direction);

	void SetTypeLightPoint();
	void SetTypeLightSpot();
	void SetTypeLightDirectional();

	void SetCurrentCamera(Camera* _camera) { _currentCamera = _camera; }

	void Draw(bool& wireFrameActive) override;


	~Light();
protected:
	bool _enableBackUpDataLight = true;
	glm::vec3 _auxColour;
	glm::vec3 _auxAmbient;
	glm::vec3 _auxDiffuse;
	glm::vec3 _auxSpecular;

	void SetEnableDrawAABB(bool value) override;
	void BindBuffer() override;
	void SetVAO();
	void SetVBO();
	void SetIBO();
	void UnbindBuffers();
	void CreateDataLight();

	void SetBoolsTypeLight();

	void SetPointLight(float linearVal, float quadraticVal);
	void SetLinearValuePoint(float value) { _linearValuePoint = value; }
	void SetQuadraticValuePoint(float value) { _quadraticValuePoint = value; }

	void SetSpotLight(float linearVal, float quadraticVal);
	void SetLinearValueSpot(float value) { _linearValueSpot = value; }
	void SetQuadraticValueSpot(float value) { _quadraticValueSpot = value; }
	void SetCutOffSpotLight(float value) { _cutOffValueSpot = value; }
	void SetOuterCutOffSpotLight(float value) { _outerCutOffValueSpot = value; }

	void SetDirectionLight(glm::vec3 direction) { _directionDirectinal = direction; }

	void SetCountLightInShader(Shader shader);
	void UpdateCountLightInShader(Shader shader);

	Camera* _currentCamera;

private:

	float lightSourceCube[lightCubeCount] = {
		//	x		y		z	
		  -1.0,   -1.0,    1.0,   1.0,   1.0,    1.0,
		   1.0,   -1.0,    1.0,   1.0,   1.0,    1.0,
		   1.0,    1.0,    1.0,   1.0,   1.0,    1.0,
		  -1.0,    1.0,    1.0,   1.0,   1.0,    1.0,
		  -1.0,   -1.0,   -1.0,   1.0,   1.0,    1.0,
		   1.0,   -1.0,   -1.0,   1.0,   1.0,    1.0,
		   1.0,    1.0,   -1.0,   1.0,   1.0,    1.0,
		  -1.0,    1.0,   -1.0,   1.0,   1.0,    1.0
	};
	unsigned int indexCubeLight[lightIndicesCubeCount] = {
	0, 1, 2,
	2, 3, 0,
	1, 5, 6,
	6, 2, 1,
	7, 6, 5,
	5, 4, 7,
	4, 0, 3,
	3, 7, 4,
	4, 5, 1,
	1, 0, 4,
	3, 2, 6,
	6, 7, 3
	};


	//=========================================
	//Directional
	unsigned int _uniformDirectionLocation;
	unsigned int _uniformAmbDirectionalLoc;
	unsigned int _uniformDiffDirectionalLoc;
	unsigned int _uniformSpecDirectionalLoc;

	unsigned int _unifromColourDirectional;
	//=========================================
	//Point
	unsigned int _uniformConstPointLight;
	unsigned int _uniformQuadraticPointLight;
	unsigned int _uniformLinearPointLight;
	unsigned int _uniformPosLightPointLoc;

	unsigned int _uniformAmbientPointLoc;
	unsigned int _uniformDiffusePointLoc;
	unsigned int _uniformSpecularPointLoc;

	unsigned int _unifromColourPoint;
	//=========================================
	//Spot
	unsigned int _uniformOuterCutOffSpotLight;
	unsigned int _uniformCutOffSpotLight;
	unsigned int _uniformPosLightSpotLoc;
	unsigned int _uniformAmbientSpotLoc;
	unsigned int _uniformDiffuseSpotLoc;
	unsigned int _uniformSpecularSpotLoc;
	unsigned int _uniformSpotLightDirection;

	unsigned int _uniformQuadraticSpotLight;
	unsigned int _uniformLinearSpotLight;
	unsigned int _uniformConstSpotLight;

	unsigned int _unifromColourSpot;

	//Camera pos
	unsigned int _uniformPosCameraLocation;

	//DIRECTIONAL
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	glm::vec3 _directionDirectinal;
	glm::vec3 _directionSpot;
	unsigned int nr_of_directional_lightLocation;

	//POINT
	float _linearValuePoint;
	float _quadraticValuePoint;
	const float _constValuePoint = 1.0f;
	unsigned int nr_of_point_lightsLocation;

	//SPOT
	float _linearValueSpot;
	float _quadraticValueSpot;
	const float _constValueSpot = 1.0f;
	float _cutOffValueSpot;
	float _outerCutOffValueSpot;

	unsigned int nr_of_spot_lightLocation;

	//OTHERS
	glm::vec3 _colour;
	TypeLight _typeLight;
	int _isDirectional;
	int _isPoint;
	int _isSpot;
};

#endif