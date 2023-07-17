#include <glew.h>
#include "GLFW/glfw3.h"

#include <fstream>
#include <sstream>
#include "shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::CreateShader(const char* vertexPath, const char* fragmentPath)
{
	id = glCreateProgram();
	unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexPath);
	unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	glValidateProgram(id);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

unsigned int Shader::getId()
{
	return id;
}

void Shader::use()
{
	glUseProgram(id);
}

void Shader::deleteShader()
{
	glDeleteProgram(id);
}

unsigned int Shader::compileShader(unsigned int type, const char* source)
{
	unsigned int id = glCreateShader(type);
	std::string shaderSourceCode;
	std::ifstream shaderSourceFile;

	shaderSourceFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		shaderSourceFile.open(source);
		std::stringstream sourceShaderStream;

		sourceShaderStream << shaderSourceFile.rdbuf();

		shaderSourceFile.close();

		shaderSourceCode = sourceShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_OPEN" << std::endl;
	}

	const char* sourceCode = shaderSourceCode.c_str();

	glShaderSource(id, 1, &sourceCode, nullptr);
	glCompileShader(id);

	return id;
}
