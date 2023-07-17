#include "TextureImporter.h"

#include <iostream>

#include <glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION


TextureImporter::TextureImporter() {

}
TextureImporter::~TextureImporter() {
}
bool TextureImporter::LoadTexture(const char* path, unsigned char* data, unsigned int& texture, int width, int height, int channels, bool transparent) {
	stbi_set_flip_vertically_on_load(true);

	data = stbi_load(path, &width, &height, &channels, 0);
	if (!data) {
		std::cout << "No Carga Textura" << std::endl;
		return false;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (channels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	return true;
}