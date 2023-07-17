#ifndef TEXTUREIMPORTER_H
#define TEXTUREIMPORTER_H

#include "PrivateClass/Export.h"

class ENGINE_API TextureImporter {
public:
	TextureImporter();
	~TextureImporter();
	bool LoadTexture(const char* path, unsigned char* data, unsigned int& texture, int width, int height, int channels, bool transparent);
};
#endif