#include "CollisionManager.h"

#include "AxisAlignedBoundingBox.h"
#include "Camera.h"
#include <math.h>

CollisionManager::CollisionManager() {}

CollisionManager::~CollisionManager() {}


CollisionResult2D CollisionManager::ParcialCollisionDetection2D(Entity2D* myEntity, Entity2D* toCheckTrigger2D, glm::vec3 myScale, glm::vec3 toCheckScale)
{
	float toCheckPosX = toCheckTrigger2D->transform.position.x;
	float toCheckPosY = toCheckTrigger2D->transform.position.y;

	float toCheckScaleX = toCheckScale.x;
	float toCheckScaleY = toCheckScale.y;

	float myPosX = myEntity->transform.position.x;
	float myPosY = myEntity->transform.position.y;

	float myScaleX = myScale.x;
	float myScaleY = myScale.y;

	float minOverlapX = 0.0f;
	float maxOverlapX = glm::min(myPosX + fabs(myScaleX) / 2.0f, toCheckPosX + fabs(toCheckScaleX) / 2.0f) - glm::max(myPosX - fabs(myScaleX) / 2.0f, toCheckPosX - fabs(toCheckScaleX) / 2.0f);

	float overlapX = glm::max(minOverlapX, maxOverlapX);

	float minOverlapY = 0.0f;
	float maxOverlapY = glm::min(myPosY + fabs(myScaleY) / 2.0f, toCheckPosY + fabs(toCheckScaleY) / 2.0f) - glm::max(myPosY - fabs(myScaleY) / 2.0f, toCheckPosY - fabs(toCheckScaleY) / 2.0f);

	float overlapY = glm::max(minOverlapY, maxOverlapY);

	if (overlapX != 0.0f && overlapY != 0.0f)
	{
		if (overlapX > overlapY)
		{
			if (myPosY < 0 && myPosY < toCheckPosY || myPosY > 0 && myPosY < toCheckPosY) {
				std::cout << "CollisionUp" << std::endl;
				return CollisionUp;
			}
			else if (myPosY < 0 && myPosY > toCheckPosY || myPosY > 0 && myPosY > toCheckPosY) {
				std::cout << "CollisionDown" << std::endl;
				return CollisionDown;
			}
		}
		else
		{
			if (myPosX < 0 && myPosX < toCheckPosX || myPosX > 0 && myPosX < toCheckPosX) {
				std::cout << "CollisionRight" << std::endl;
				return CollisionRight;
			}
			else if (myPosX < 0 && myPosX > toCheckPosX || myPosX > 0 && myPosX > toCheckPosX) {
				std::cout << "CollisionLeft" << std::endl;
				return CollisionLeft;
			}
		}
	}

	return NoneCollision;
}

bool CollisionManager::CheckTrigger2D(Entity2D* myEntity, Entity2D* toCheckEntity2D, glm::vec3 myScale, glm::vec3 toCheckScale)
{
	CollisionResult2D collisionResult = ParcialCollisionDetection2D(myEntity, toCheckEntity2D, myScale, toCheckScale);

	if (collisionResult != CollisionResult2D::NoneCollision)
		return true;

	return false;
}

bool CollisionManager::CheckCollision2D(Entity2D* myEntity, Entity2D* toCheckEntity2D, float speedMyEntity, glm::vec3 myScale, glm::vec3 toCheckScale)
{
	if (speedMyEntity <= 0)
		speedMyEntity = 0.05f;

	CollisionResult2D collisionResult = ParcialCollisionDetection2D(myEntity, toCheckEntity2D, myScale, toCheckScale);
	if (collisionResult != CollisionResult2D::NoneCollision)
	{
		switch (collisionResult)
		{
		case CollisionResult2D::CollisionDown:
			myEntity->SetPosition(myEntity->transform.position.x,
				myEntity->transform.position.y + speedMyEntity,
				myEntity->transform.position.z);
			break;
		case CollisionResult2D::CollisionLeft:
			myEntity->SetPosition(myEntity->transform.position.x + speedMyEntity,
				myEntity->transform.position.y,
				myEntity->transform.position.z);
			break;
		case CollisionResult2D::CollisionRight:
			myEntity->SetPosition(myEntity->transform.position.x - speedMyEntity,
				myEntity->transform.position.y,
				myEntity->transform.position.z);
			break;
		case CollisionResult2D::CollisionUp:
			myEntity->SetPosition(myEntity->transform.position.x,
				myEntity->transform.position.y - speedMyEntity,
				myEntity->transform.position.z);
			break;
		}

		return true;
	}

	return false;
}