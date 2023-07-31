#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H
#include "PrivateClass/Export.h"

#include "Entity.h"
#include "Entity2D.h"

enum CollisionResult2D
{
	CollisionLeft,
	CollisionRight,
	CollisionUp,
	CollisionDown,
	NoneCollision
};

class ENGINE_API CollisionManager
{

private:
	CollisionResult2D ParcialCollisionDetection2D(Entity2D* myEntity, Entity2D* toCheckTrigger2D, glm::vec3 myScale, glm::vec3 toCheckScale);
public:
	CollisionManager();
	~CollisionManager();

	bool CheckTrigger2D(Entity2D* myEntity, Entity2D* toCheckEntity2D, glm::vec3 myScale, glm::vec3 toCheckScale);
	bool CheckCollision2D(Entity2D* myEntity, Entity2D* toCheckEntity2D, float speedMyEntity, glm::vec3 myScale, glm::vec3 toCheckScale);
};

#endif
