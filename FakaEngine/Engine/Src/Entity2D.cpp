#include "Entity2D.h"
#include "AxisAlignedBoundingBox.h"

Entity2D::Entity2D(Renderer* _renderer, Material* _material) : Entity(_renderer)
{
	renderer = _renderer;
	material = _material;
	boxColliderSize2D = transform.scale;
}

Entity2D::Entity2D(Renderer* _renderer) : Entity(_renderer)
{
	boxColliderSize2D = transform.scale;
}

Entity2D::~Entity2D()
{
}

void Entity2D::BindBuffer() {}

void Entity2D::SetEnableDrawAABB(bool value)
{
	if (axisAlignedBoundingBox != NULL)
		axisAlignedBoundingBox->SetEnableDraw(value);
}

void Entity2D::SetScale(float x, float y, float z)
{
	transform.scale[0] = x;
	transform.scale[1] = y;
	transform.scale[2] = z;

	internalData.scale = glm::scale(glm::mat4(1.0f), transform.scale);
	UpdateMatrixModel();
	SetSizeCollider2D(glm::vec3(x, y, z));
}

string Entity2D::GetNameOfClass()
{
	return "Entity2D";
}
