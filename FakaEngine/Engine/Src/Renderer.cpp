#include "Renderer.h"
#include <iostream>

#include <glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>

#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Window.h"

Renderer::Renderer() {
}
Renderer::~Renderer() {
	_shaderUse.deleteShader();
}

void Renderer::SetCurrentShaderUse(const char* pathVertex, const char* pathFragment)
{
	_shaderUse.CreateShader(pathVertex, pathFragment);
}

void Renderer::GLEWInit() {
	glewExperimental = GL_TRUE;
	glewInit();
	if (glewInit() != GLEW_OK)
		std::cout << "Error on GLEW!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
}

void Renderer::GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool Renderer::GLLogCall() {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error]	(" << error << ")" << std::endl;
		return false;
	}
	return true;
}

void Renderer::UseShaderEnt(Shader& shader, glm::mat4 model)
{
	unsigned int modelLocation = glGetUniformLocation(shader.getId(), "model");
	glUseProgram(shader.getId());
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
}

void Renderer::ClearShader() {
	glUseProgram(0);
}

void Renderer::UnbindBuffer() {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	ClearShader();
}

void Renderer::SetLighting(Light* _light, int iteration)
{
	if (_light != NULL)
	{
#pragma region TYPES LIGHT
#pragma endregion
		switch (_light->GetTypeLight())
		{
		case Light::TypeLight::Directional:
#pragma region DIRECTIONAL LIGHT
			_light->SetUniformDirectionLightLocation(GetCurrentShaderUse(), iteration);
			_light->SetUniformAmbDirectionalLoc(GetCurrentShaderUse(), iteration);
			_light->SetUniformDiffDirectionalLoc(GetCurrentShaderUse(), iteration);
			_light->SetUniformSpecDirectionalLoc(GetCurrentShaderUse(), iteration);
			_light->SetUniformColourDirectional(GetCurrentShaderUse(), iteration);
#pragma endregion
			break;
		case Light::TypeLight::Point:
#pragma region POINT LIGHT
			_light->SetUniformPosLightPoint(GetCurrentShaderUse(), iteration);
			_light->SetUniformQuadraticPointLight(GetCurrentShaderUse(), iteration);
			_light->SetUniformLinearPointLight(GetCurrentShaderUse(), iteration);
			_light->SetUniformConstPointLight(GetCurrentShaderUse(), iteration);
			_light->SetUniformAmbientPointLoc(GetCurrentShaderUse(), iteration);
			_light->SetUniformDiffPointLoc(GetCurrentShaderUse(), iteration);
			_light->SetUniformSpecPointLoc(GetCurrentShaderUse(), iteration);
			_light->SetUniformColourPoint(GetCurrentShaderUse(), iteration);
#pragma endregion
			break;
		case Light::TypeLight::Spot:
#pragma region SPOT LIGHT
			_light->SetUniformPosLightSpot(GetCurrentShaderUse(), iteration);
			_light->SetUniformOuterCutOffSopttLight(GetCurrentShaderUse(), iteration);
			_light->SetUniformCutOffSopttLight(GetCurrentShaderUse(), iteration);
			_light->SetUniformLinearSpotLight(GetCurrentShaderUse(), iteration);
			_light->SetUniformQuadraticSpotLight(GetCurrentShaderUse(), iteration);
			_light->SetUniformConstSpotLight(GetCurrentShaderUse(), iteration);
			_light->SetUniformAmbientSpotLoc(GetCurrentShaderUse(), iteration);
			_light->SetUniformDiffSpotLoc(GetCurrentShaderUse(), iteration);
			_light->SetUniformSpecSpotLoc(GetCurrentShaderUse(), iteration);
			_light->SetUniformSpotLightDirection(GetCurrentShaderUse(), iteration);
			_light->SetUniformColourSpot(GetCurrentShaderUse(), iteration);
#pragma endregion
			break;
		}
#pragma region BASIC LIGHTING (POS LIGHT)
		_light->SetUniformPosCameraLocation(GetCurrentShaderUse());
#pragma endregion
	}
}


void Renderer::SetMaterial(Material* _material)
{
	if (_material != NULL)
	{
		_material->SetUniformAmbientMatLocation(GetCurrentShaderUse());
		_material->SetUniformDiffuseMatLocation(GetCurrentShaderUse());
		_material->SetUniformSpecularMatLocation(GetCurrentShaderUse());
		_material->SetUniformShininessLocation(GetCurrentShaderUse());
	}
}

void Renderer::BeignDraw() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(int indices, Shader& shaderProg, glm::mat4 model, bool& wireframeActive)
{
	UseShaderEnt(shaderProg, model);

	if (!wireframeActive)
		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
	else
		glDrawElements(GL_LINE_STRIP, indices, GL_UNSIGNED_INT, 0);

	UnbindBuffer();
}

void Renderer::DrawColliders(int indices, Shader& shaderProg, glm::mat4 model, bool& drawActive, bool& isFrustrum)
{
	UseShaderEnt(shaderProg, model);

	if (drawActive)
	{
		if (!isFrustrum)
			glDrawElements(GL_LINE_STRIP, indices, GL_UNSIGNED_INT, 0);
		else
			glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
	}

	UnbindBuffer();
}

void Renderer::DrawSprite(unsigned int figura, int vertexs, unsigned int vbo, Shader& shaderProg, glm::mat4 model)
{
	UseShaderEnt(shaderProg, model);

	glDrawArrays(figura, 0, vertexs);

	UnbindBuffer();
}

void Renderer::EndDraw(Window* refWindow) {
	ClearShader();
	refWindow->SwapBuffersWindows();
}
