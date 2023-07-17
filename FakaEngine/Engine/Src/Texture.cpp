#include "Texture.h"

#include <glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

Texture::Texture(const char* path, bool transparency)
{
	_path = path;
	_texData = 0;
	_textureID = 0;
	_width = 0;
	_height = 0;
	_bitDepth = 0;
	_channels = 0;
	_transparency = transparency;
}

Texture::~Texture() 
{ 
	UnbindTexture();
	UnloadTexture();
}

bool Texture::LoadTexture(bool hasAlpha)
{
	return _myImporter.LoadTexture(_path, _texData, _textureID, _width, _height, _channels, hasAlpha);
}

bool Texture::LoadTexture(const char * path, bool hasAlpha)
{
	return _myImporter.LoadTexture(path, _texData, _textureID, _width, _height, _channels, hasAlpha);
}

void Texture::BlendTexture()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Texture::UnblendTexture()
{
	glDisable(GL_BLEND);
}

void Texture::BindTexture()
{
	if (_transparency) 
	{
		BlendTexture();
	}
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,_textureID);
}

void Texture::UnbindTexture()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	if (_transparency) 
	{
		UnblendTexture();
	}
}

void Texture::UnloadTexture()
{
	glDeleteTextures(1, &_textureID);
	_path = " ";
	_texData = 0;
	_textureID = -1;
	_width = 0;
	_height = 0;
	_bitDepth = 0;
	_channels = 0;
	std::cout << "Textura descargada" << std::endl;
}