#ifndef ENTITY2D_H
#define ENTITY2D_H

#include "Entity.h"
#include "Material.h"
#include "Renderer.h"

class ENGINE_API Entity2D : public Entity
{
protected:
	glm::vec3 boxColliderSize2D;
	void BindBuffer() override;
	void SetEnableDrawAABB(bool value) override;
public:
	Entity2D(Renderer* _renderer, Material* _material);
	Entity2D(Renderer* _renderer);
	~Entity2D();
	Material* material;
	void SetSizeCollider2D(glm::vec3 resize) { boxColliderSize2D = resize; }
	glm::vec3 GetBoxColliderSize2D() { return boxColliderSize2D; }
	void SetScale(float x, float y, float z) override;
	string GetNameOfClass() override;
};
#endif

