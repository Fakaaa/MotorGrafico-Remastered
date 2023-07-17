#ifndef TEXTURE_H
#define TEXTURE_H

#include "PrivateClass/Export.h"
#include "TextureImporter.h"

class ENGINE_API Texture
{
private:
	const char* _path;
	unsigned char* _texData;
	unsigned int _textureID;
	int _width;
	int _height;
	int _bitDepth;
	int _channels;
	bool _transparency;
	TextureImporter _myImporter;
public:
	Texture(const char* path, bool transparency);
	~Texture();
	bool LoadTexture(bool hasAlpha);
	bool LoadTexture(const char* path, bool hasAlpha);

	void BlendTexture();

	void UnblendTexture();
	unsigned int GetTexture() { return _textureID; }
	void BindTexture();
	void UnbindTexture();

	void UnloadTexture();
};

#endif // !TEXTURE_H