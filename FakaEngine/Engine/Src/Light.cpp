#include "Light.h"

#include "Camera.h"
#include "glew.h"
#include "GLFW/glfw3.h"
#include <sstream>
#include "AxisAlignedBoundingBox.h"

int Light::nr_of_directional_light = 0;
int Light::nr_of_point_light = 0;
int Light::nr_of_spot_light = 0;

Light::Light(Renderer* _render, TypeLight type, Camera* camera) : Entity(_render)
{
	_colour = glm::vec3(1.0f, 1.0f, 1.0f);
	_ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	_diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	_specular = glm::vec3(0.0f, 0.0f, 0.0f);
	_typeLight = type;
	_currentCamera = camera;
	//SetBoolsTypeLight();

	switch (_typeLight)
	{
	case TypeLight::Directional:
		nr_of_directional_light++;
		SetDirectionLight(glm::vec3(-0.2f, -1.0f, -2.3f));
		break;
	case TypeLight::Point:
		nr_of_point_light++;
		SetTypeLightPoint();
		break;
	case TypeLight::Spot:
		nr_of_spot_light++;
		SetTypeLightSpot();
		break;
	default:
		break;
	}

	InmortalObject = true;

	UpdateCountLightInShader(_render->GetCurrentShaderUse());

	CreateDataLight();
}

Light::Light(glm::vec3 colour, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Renderer* render, TypeLight type, Camera* camera) : Entity(render)
{
	_colour = colour;
	_ambient = ambient;
	_diffuse = diffuse;
	_specular = specular;
	_typeLight = type;
	_currentCamera = camera;
	SetBoolsTypeLight();

	switch (_typeLight)
	{
	case TypeLight::Directional:
		nr_of_directional_light++;
		SetDirectionLight(glm::vec3(-0.2f, -1.0f, -2.3f));
		break;
	case TypeLight::Point:
		nr_of_point_light++;
		SetTypeLightPoint();
		break;
	case TypeLight::Spot:
		nr_of_spot_light++;
		SetTypeLightSpot();
		break;
	default:
		break;
	}

	InmortalObject = true;

	UpdateCountLightInShader(render->GetCurrentShaderUse());

	CreateDataLight();
}
Light::~Light()
{
	switch (_typeLight)
	{
	case Light::Point:
		nr_of_point_light--;
		break;
	case Light::Directional:
		nr_of_directional_light--;
		break;
	case Light::Spot:
		nr_of_spot_light--;
		break;
	}
	UpdateCountLightInShader(renderer->GetCurrentShaderUse());
}

void Light::SetEnableDrawAABB(bool value)
{
	if (axisAlignedBoundingBox != NULL)
		axisAlignedBoundingBox->SetEnableDraw(value);
}

void Light::BindBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glVertexAttribPointer(_positionLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(_positionLocation);
	glVertexAttribPointer(_colorLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(_colorLocation);
}

void Light::Draw(bool& wireFrameActive)
{
	if (!isAlive && _enableBackUpDataLight) {
		_auxColour = _colour;
		_auxAmbient = _ambient;
		_auxDiffuse = _diffuse;
		_auxSpecular = _specular;
		SetAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
		SetDiffuse(glm::vec3(0.0f, 0.0f, 0.0f));
		SetSpecular(glm::vec3(0.0f, 0.0f, 0.0f));
		_enableBackUpDataLight = false;
	}
	UseLight();

	if (isAlive) {
		if (!_enableBackUpDataLight) {
			_colour = _auxColour;
			_ambient = _auxAmbient;
			_diffuse = _auxDiffuse;
			_specular = _auxSpecular;
			_enableBackUpDataLight = true;
		}

		renderer->Draw(lightIndicesCubeCount, renderer->GetCurrentShaderUse(), internalData.localModel, wireFrameActive);
	}
}

void Light::UseLight()
{
	CheckIsModel();

	BindBuffer();

	renderer->GetCurrentShaderUse().use();

	switch (_typeLight)
	{
	case Light::Point:
#pragma region POINT LIGHT
		glUniform3f(_uniformPosLightPointLoc, this->transform.position.x, this->transform.position.y, this->transform.position.z);

		glUniform1f(_uniformConstPointLight, _constValuePoint);

		glUniform1f(_uniformLinearPointLight, _linearValuePoint);

		glUniform1f(_uniformQuadraticPointLight, _quadraticValuePoint);

		glUniform3f(_uniformAmbientPointLoc, _ambient.x, _ambient.y, _ambient.z);

		glUniform3f(_uniformDiffusePointLoc, _diffuse.x, _diffuse.y, _diffuse.z);

		glUniform3f(_uniformSpecularPointLoc, _specular.x, _specular.y, _specular.z);

		glUniform3f(_unifromColourPoint, _colour.x, _colour.y, _colour.z);
#pragma endregion
		break;
	case Light::Directional:
#pragma region DIRECTIONAL LIGHT
		glUniform3f(_uniformDirectionLocation, _directionDirectinal.x, _directionDirectinal.y, _directionDirectinal.z);

		glUniform3f(_uniformAmbDirectionalLoc, _ambient.x, _ambient.y, _ambient.z);

		glUniform3f(_uniformDiffDirectionalLoc, _diffuse.x, _diffuse.y, _diffuse.z);

		glUniform3f(_uniformSpecDirectionalLoc, _specular.x, _specular.y, _specular.z);

		glUniform3f(_unifromColourDirectional, _colour.x, _colour.y, _colour.z);
#pragma endregion
		break;
	case Light::Spot:
#pragma region SPOT LIGHT
		glUniform3f(_uniformPosLightSpotLoc, this->transform.position.x, this->transform.position.y, this->transform.position.z);

		glUniform1f(_uniformCutOffSpotLight, glm::cos(glm::radians(_cutOffValueSpot)));

		glUniform1f(_uniformOuterCutOffSpotLight, glm::cos(glm::radians(_outerCutOffValueSpot)));

		glUniform1f(_uniformConstSpotLight, _constValueSpot);

		glUniform1f(_uniformLinearSpotLight, _linearValueSpot);

		glUniform1f(_uniformQuadraticSpotLight, _quadraticValueSpot);

		glUniform3f(_uniformAmbientSpotLoc, _ambient.x, _ambient.y, _ambient.z);

		glUniform3f(_uniformDiffuseSpotLoc, _diffuse.x, _diffuse.y, _diffuse.z);

		glUniform3f(_uniformSpecularSpotLoc, _specular.x, _specular.y, _specular.z);

		glUniform3f(_uniformSpotLightDirection, transform.backward.x, transform.backward.y, transform.backward.z);

		glUniform3f(_unifromColourSpot, _colour.x, _colour.y, _colour.z);
#pragma endregion
		break;
	}

#pragma region BASIC LIGHTING (POS LIGHT)
	glUniform3f(_uniformPosCameraLocation, _currentCamera->transform.position.x,
		_currentCamera->transform.position.y, _currentCamera->transform.position.z);
#pragma endregion

	glUseProgram(0);
}
string Light::GetClassName()
{
	return "Light";
}
void Light::CreateDataLight()
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
void Light::SetColorLight(float r, float g, float b)
{
	_colour = glm::vec3(r, g, b);
}

glm::vec3 Light::GetColorLight()
{
	return _colour;
}

void Light::SetTypeLightDefault(TypeLight type)
{
	switch (type)
	{
	case Light::Point:
		SetTypeLightPoint();
		break;
	case Light::Directional:
		SetTypeLightDirectional();
		break;
	case Light::Spot:
		SetTypeLightSpot();
		break;
	}
}

void Light::SetPointLightCustom(float linearVal, float quadraticVal, float cutOffValue)
{
	SetTypeLightPoint(linearVal, quadraticVal, cutOffValue);
}

void Light::SetSpotLightCustom(float linearVal, float quadraticVal, float cutOffValue, float outerCutOffValue)
{
	SetTypeLightSpot(linearVal, quadraticVal, cutOffValue, outerCutOffValue);
}

void Light::SetDirectionalLightCustom(glm::vec3 direction)
{
	SetTypeLightDirectional(direction);
}
string Light::GetTypeLightString()
{
	string localTypeLightString = "None";
	switch (_typeLight)
	{
	case Light::Point:
		localTypeLightString = "Point";
		break;
	case Light::Directional:
		localTypeLightString = "Directional";
		break;
	case Light::Spot:
		localTypeLightString = "Spot";
		break;
	}
	return localTypeLightString;
}
//================================================
//DIRECTIONAL LIGHT


void Light::SetTypeLightPoint(float linearVal, float quadraticVal, float cutOffValue)
{
	SetPointLight(linearVal, quadraticVal);
	SetCutOffSpotLight(cutOffValue);

	if (_typeLight != TypeLight::Point)
		nr_of_point_light++;

	_typeLight = TypeLight::Point;

	UpdateCountLightInShader(renderer->GetCurrentShaderUse());

}

void Light::SetTypeLightPoint()
{
	SetPointLight(0.0014f, 0.000007f);

	if (_typeLight != TypeLight::Point)
		nr_of_point_light++;

	_typeLight = TypeLight::Point;

	UpdateCountLightInShader(renderer->GetCurrentShaderUse());

	SetBoolsTypeLight();
}

void Light::SetTypeLightSpot(float linearVal, float quadraticVal, float cutOffValue, float outerCutOffValue)
{
	SetPointLight(linearVal, quadraticVal);
	SetCutOffSpotLight(cutOffValue);
	SetOuterCutOffSpotLight(outerCutOffValue);

	if (_typeLight != TypeLight::Spot)
		nr_of_spot_light++;

	_typeLight = TypeLight::Spot;

	UpdateCountLightInShader(renderer->GetCurrentShaderUse());

	SetBoolsTypeLight();
}

void Light::SetTypeLightSpot()
{
	SetSpotLight(0.0014f, 0.000007f);
	SetCutOffSpotLight(12.5f);
	SetOuterCutOffSpotLight(17.5f);

	if (_typeLight != TypeLight::Spot)
		nr_of_spot_light++;

	_typeLight = TypeLight::Spot;

	UpdateCountLightInShader(renderer->GetCurrentShaderUse());

	SetBoolsTypeLight();
}

void Light::SetTypeLightDirectional()
{
	SetDirectionLight(glm::vec3(-0.2f, -1.0f, -2.3f));

	if (_typeLight != TypeLight::Directional)
		nr_of_directional_light++;

	_typeLight = TypeLight::Directional;

	UpdateCountLightInShader(renderer->GetCurrentShaderUse());

	SetBoolsTypeLight();
}

void Light::SetTypeLightDirectional(glm::vec3 direction)
{
	SetDirectionLight(direction);

	if (_typeLight != TypeLight::Directional)
		nr_of_directional_light++;

	_typeLight = TypeLight::Directional;

	UpdateCountLightInShader(renderer->GetCurrentShaderUse());

	SetBoolsTypeLight();
}

void Light::SetPointLight(float linearVal, float quadraticVal)
{
	SetLinearValuePoint(linearVal);
	SetQuadraticValuePoint(quadraticVal);
}

void Light::SetSpotLight(float linearVal, float quadraticVal)
{
	SetLinearValueSpot(linearVal);
	SetQuadraticValueSpot(quadraticVal);
}

void Light::SetBoolsTypeLight()
{
	switch (_typeLight)
	{
	case Light::Point:
		_isPoint = 1;
		_isDirectional = 0;
		_isSpot = 0;
		break;
	case Light::Directional:
		_isDirectional = 1;
		_isPoint = 0;
		_isSpot = 0;
		break;
	case Light::Spot:
		_isSpot = 1;
		_isDirectional = 0;
		_isPoint = 0;
		break;
	}
}

#pragma region FUNCIONES DE INICIALIZACION DE DATA

void Light::SetVAO()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
}

void Light::SetVBO()
{
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightSourceCube), lightSourceCube, GL_STATIC_DRAW);

	_positionLocation = glGetAttribLocation(renderer->GetCurrentShaderUse().getId(), "position");

	_colorLocation = glGetAttribLocation(renderer->GetCurrentShaderUse().getId(), "customColor");

	BindBuffer();
}

void Light::SetIBO()
{
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexCubeLight), indexCubeLight, GL_STATIC_DRAW);
}

void Light::UnbindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Light::SetCountLightInShader(Shader shader)
{
	nr_of_directional_lightLocation = glGetUniformLocation(shader.getId(), "nr_of_directional_light");
	nr_of_point_lightsLocation = glGetUniformLocation(shader.getId(), "nr_of_point_lights");
	nr_of_spot_lightLocation = glGetUniformLocation(shader.getId(), "nr_of_spot_light");
}

void Light::UpdateCountLightInShader(Shader shader)
{
	CheckIsModel();
	renderer->GetCurrentShaderUse().use();
	SetCountLightInShader(shader);
	glUniform1i(nr_of_directional_lightLocation, nr_of_directional_light);
	glUniform1i(nr_of_point_lightsLocation, nr_of_point_light);
	glUniform1i(nr_of_spot_lightLocation, nr_of_spot_light);
	glUseProgram(0);
}

#pragma region DIRECTIONAL_LIGHT

void Light::SetUniformColourDirectional(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "dirLight[" + iterS.str() + "].colour";

	_unifromColourDirectional = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformAmbDirectionalLoc(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "dirLight[" + iterS.str() + "].ambient";

	_uniformAmbDirectionalLoc = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformDiffDirectionalLoc(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "dirLight[" + iterS.str() + "].diffuse";

	_uniformDiffDirectionalLoc = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformSpecDirectionalLoc(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "dirLight[" + iterS.str() + "].specular";

	_uniformSpecDirectionalLoc = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformDirectionLightLocation(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "dirLight[" + iterS.str() + "].direction";

	_uniformDirectionLocation = glGetUniformLocation(shader.getId(), result.c_str());
}

#pragma endregion

#pragma region POINT_LIGHT

void Light::SetUniformConstPointLight(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "pLight[" + iterS.str() + "].constant";

	_uniformConstPointLight = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformAmbientPointLoc(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "pLight[" + iterS.str() + "].ambient";

	_uniformAmbientPointLoc = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformDiffPointLoc(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "pLight[" + iterS.str() + "].diffuse";

	_uniformDiffusePointLoc = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformSpecPointLoc(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "pLight[" + iterS.str() + "].specular";

	_uniformSpecularPointLoc = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformLinearPointLight(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "pLight[" + iterS.str() + "].linear";

	_uniformLinearPointLight = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformQuadraticPointLight(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "pLight[" + iterS.str() + "].quadratic";

	_uniformQuadraticPointLight = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformPosLightPoint(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "pLight[" + iterS.str() + "].posLight";

	_uniformPosLightPointLoc = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformColourPoint(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "pLight[" + iterS.str() + "].colour";

	_unifromColourPoint = glGetUniformLocation(shader.getId(), result.c_str());
}

#pragma endregion

#pragma region SPOT_LIGHT

void Light::SetUniformCutOffSopttLight(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "sLight[" + iterS.str() + "].cutOff";

	_uniformCutOffSpotLight = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformOuterCutOffSopttLight(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "sLight[" + iterS.str() + "].outerCutOff";

	_uniformOuterCutOffSpotLight = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformPosLightSpot(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "sLight[" + iterS.str() + "].posLight";

	_uniformPosLightSpotLoc = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformSpotLightDirection(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "sLight[" + iterS.str() + "].direction";

	_uniformSpotLightDirection = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformColourSpot(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "sLight[" + iterS.str() + "].colour";

	_unifromColourSpot = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformAmbientSpotLoc(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "sLight[" + iterS.str() + "].ambient";

	_uniformAmbientSpotLoc = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformDiffSpotLoc(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "sLight[" + iterS.str() + "].diffuse";

	_uniformDiffuseSpotLoc = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformSpecSpotLoc(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "sLight[" + iterS.str() + "].specular";

	_uniformSpecularSpotLoc = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformLinearSpotLight(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "sLight[" + iterS.str() + "].linear";

	_uniformLinearSpotLight = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformQuadraticSpotLight(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "sLight[" + iterS.str() + "].quadratic";

	_uniformQuadraticSpotLight = glGetUniformLocation(shader.getId(), result.c_str());
}

void Light::SetUniformConstSpotLight(Shader& shader, int iter)
{
	stringstream iterS;
	iterS << iter;
	string result = "sLight[" + iterS.str() + "].constant";

	_uniformConstSpotLight = glGetUniformLocation(shader.getId(), result.c_str());
}

#pragma endregion

void Light::SetUniformPosCameraLocation(Shader& shader)
{
	_uniformPosCameraLocation = glGetUniformLocation(shader.getId(), "cameraPos");
}
#pragma endregion