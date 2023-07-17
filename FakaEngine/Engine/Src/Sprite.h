#ifndef SPRITE_H
#define SPRITE_H
#include "Entity2D.h"
#include "TextureImporter.h"
#include "Animation.h"
#include "Timer.h"

#define COUNT_TEXTURE_VERTEX_COORD 4

class ENGINE_API Sprite : public Entity2D
{
private:
	float textureVertex[20] = {
		//    x     y     z     u     v
		   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f
	};

	unsigned int texture;
	unsigned char* data;
	int width;
	int height;
	int nrChannels;
	TextureImporter* texImporter;
	bool _transparency;
	Animation* animation = NULL;
	int _currentFrame;
	int _previusFrame;
	unsigned int _vbo;

	void InitTextureVertexCoord();
	void BindBufferSprite();
protected:
	void BindBuffer() override;
	void SetEnableDrawAABB(bool value) override;
	void UpdateSprite(Time& timer);
public:
	void Draw(bool& wireFrameActive) override;
	
	void SetAttribsSprite();

	Sprite(Renderer * _renderer, const char* filePath, bool transparency);
	~Sprite();
	void Draw(Time& timer);
	void SetTextureCoordinates(float u0, float v0,
							   float u1, float v1,
							   float u2, float v2,
		                       float u3, float v3);
	TextureImporter* GetTextureImporter() { return texImporter; }
	void SetAnimation(Animation* _animation);
	Animation* GetAnimation() { return animation; }
	int getWidth();
	int getHeigth();
	int getNrChannels();
	void SetCurrentAnimationIndex(int currentAnimation) { if (animation != NULL) animation->SetCurrentAnimation(currentAnimation); }

	string GetClassName() override;

private:
	void BlendSprite();
	void UnBlendSprite();
	void LoadTexture(const char* path, bool transparent);
};
#endif