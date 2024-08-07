#ifndef SHADER_H
#define SHADER_H

#include "PrivateClass/Export.h"
#include <iostream>

static enum ShaderType {
	VERTEX,
	FRAGMENT
};

using namespace std;
class ENGINE_API Shader 
{
private:
	void ValidateShaderCompilation(unsigned int shader, ShaderType typeShader);
	void ValidateShaderLinking(unsigned int shaderProgram);
public:
	unsigned int id;
	Shader();
	~Shader();
	void CreateShader(const char* vertexPath, const char* fragmentPath);
	unsigned int getId();
	void use();
	void deleteShader();
	unsigned int compileShader(unsigned int type, const char* source);
};
#endif // !SHADER_H