#include <glew.h>
#include "GLFW/glfw3.h"

#include <fstream>
#include <sstream>
#include "shader.h"

#pragma region PUBLIC_METHODS


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

	//Check if link process has succeed.
	ValidateShaderLinking(id);

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

	//Check if compilation  succeed.
	ValidateShaderCompilation(id, type == GL_VERTEX_SHADER ? ShaderType::VERTEX : ShaderType::FRAGMENT);

	return id;
}

#pragma endregion

#pragma region PRIVATE_METHODS

void Shader::ValidateShaderCompilation(unsigned int shader, ShaderType typeShader)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		switch (typeShader)
		{
		case VERTEX:
			cout<<"ERROR:SHADER:VERTEX:COMPILATION_FAILED"<<endl;
			break;
		case FRAGMENT:
			cout << "ERROR:SHADER:FRAGMENT:COMPILATION_FAILED" << endl;
			break;
		}
	}
	else {
		switch (typeShader)
		{
		case VERTEX:
			cout << "SUCCESS:SHADER:VERTEX:COMPILATION" << endl;
			break;
		case FRAGMENT:
			cout << "SUCCESS:SHADER:FRAGMENT:COMPILATION" << endl;
			break;
		}
	}
}

void Shader::ValidateShaderLinking(unsigned int shaderProgram)
{
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR:SHADER:LINK_FAILED" << endl;
	}
	else {
		cout << "SUCCESS:SHADER:LINK" << endl;
	}
}

#pragma endregion