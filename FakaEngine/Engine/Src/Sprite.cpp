#include "Sprite.h"
#include "glew.h"
#include "AxisAlignedBoundingBox.h"

//============================================
Sprite::Sprite(Renderer* _renderer, const char* filePath, bool transparency) :Entity2D(_renderer)
{
	renderer = _renderer;
	_transparency = transparency;

	texImporter = new TextureImporter();

	SetAttribsSprite();

	InitTextureVertexCoord();

	LoadTexture(filePath, _transparency);
}
//============================================
Sprite::~Sprite() {
	glDeleteTextures(1, &texture);
	if (texImporter != NULL) {
		delete texImporter;
		texImporter = NULL;
	}
	if (animation != NULL)
	{
		delete animation;
		animation = NULL;
	}

}

void Sprite::BindBufferSprite()
{
	unsigned int countElementsForVertex = 5;

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(_positionLocation, 3, GL_FLOAT, GL_FALSE, countElementsForVertex * sizeof(float), 0);
	glEnableVertexAttribArray(_positionLocation);
	glVertexAttribPointer(_texLocation, 2, GL_FLOAT, GL_FALSE, countElementsForVertex * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(_texLocation);
}

//============================================

void Sprite::SetAttribsSprite()
{
	int countElementsForVertex = 5;

	_positionLocation = glGetAttribLocation(renderer->GetCurrentShaderUse().getId(), "position");
	glVertexAttribPointer(_positionLocation, 3, GL_FLOAT, GL_FALSE, countElementsForVertex * sizeof(float), 0);
	glEnableVertexAttribArray(_positionLocation);
	glUniform1i(_texLocation = glGetUniformLocation(renderer->GetCurrentShaderUse().getId(), "ourTexture"), 0);
	glVertexAttribPointer(_texLocation, 2, GL_FLOAT, GL_FALSE, countElementsForVertex * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(_texLocation);
}

void Sprite::BindBuffer() {}

void Sprite::SetEnableDrawAABB(bool value)
{
	if (axisAlignedBoundingBox != NULL)
		axisAlignedBoundingBox->SetEnableDraw(value);
}

void Sprite::Draw(bool& wireFrameActive) {}

void Sprite::Draw(Time& timer)
{
	if (isAlive) {
		UpdateSprite(timer);

		BindBufferSprite();
		if (renderer != NULL)
		{
			CheckIsModel();

			if (_transparency)
				BlendSprite();

			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			renderer->DrawSprite(GL_QUADS, 4, _vbo, renderer->GetCurrentShaderUse(), internalData.localModel);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);

			if (_transparency)
				UnBlendSprite();
		}
	}
}
//============================================
void Sprite::UpdateSprite(Time& timer)
{
	if (!animation) {
		//cout << "ANIMACION NULA" << endl;
		return;
	}
	//cout << "TODAS PUTAS UWU" << endl;
	animation->Update(timer);

	_currentFrame = animation->GetCurrentFrame();
	if (_currentFrame != _previusFrame
		|| _currentFrame == animation->GetAnimation().size() - 1
		|| _currentFrame == 0) {
		SetTextureCoordinates(animation->GetAnimation()[_currentFrame].frameCoords[0].U, animation->GetAnimation()[_currentFrame].frameCoords[0].V,
			animation->GetAnimation()[_currentFrame].frameCoords[1].U, animation->GetAnimation()[_currentFrame].frameCoords[1].V,
			animation->GetAnimation()[_currentFrame].frameCoords[2].U, animation->GetAnimation()[_currentFrame].frameCoords[2].V,
			animation->GetAnimation()[_currentFrame].frameCoords[3].U, animation->GetAnimation()[_currentFrame].frameCoords[3].V);
		_previusFrame = _currentFrame;
	}
	SetAnimation(animation);
}
//============================================
void Sprite::SetAnimation(Animation* _animation)
{
	animation = _animation;
	_previusFrame = std::numeric_limits<unsigned int>::max_digits10;
}
void Sprite::BlendSprite() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Sprite::UnBlendSprite() {
	glDisable(GL_BLEND);
}
void Sprite::LoadTexture(const char* path, bool transparent) {
	_transparency = transparent;
	texImporter->LoadTexture(path, data, texture, width, height, nrChannels, _transparency);
}
//============================================
void Sprite::InitTextureVertexCoord()
{
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), textureVertex, GL_STATIC_DRAW);
}
//============================================
void Sprite::SetTextureCoordinates(float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3)
{
	textureVertex[3] = u0;
	textureVertex[8] = u1;
	textureVertex[13] = u2;
	textureVertex[18] = u3;

	textureVertex[4] = v0;
	textureVertex[9] = v1;
	textureVertex[14] = v2;
	textureVertex[19] = v3;

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), textureVertex, GL_STATIC_DRAW);
}
//============================================
int Sprite::getWidth()
{
	return width;
}
//============================================
int Sprite::getHeigth()
{
	return height;
}
//============================================
int Sprite::getNrChannels()
{
	return nrChannels;
}
string Sprite::GetNameOfClass()
{
	return "Sprite";
}
//============================================