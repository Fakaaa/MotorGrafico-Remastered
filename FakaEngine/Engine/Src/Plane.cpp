#include "Plane.h"
#include <math.h>

MyPlane::MyPlane(glm::vec3 angleLockA, glm::vec3 angleLockB)
{
	this->_angleLockA = angleLockA;
	this->_angleLockB = angleLockB;
}

MyPlane::~MyPlane() {}

bool MyPlane::CheckObjectPass(glm::vec3 meshMinColl, glm::vec3 meshMaxColl, glm::vec3 position, glm::vec3 planeFoward)
{
	glm::vec3 dirA = glm::normalize(meshMinColl - position);
	float dotProdA = glm::dot(dirA, planeFoward);

	glm::vec3 dirB = glm::normalize(meshMaxColl - position);
	float dotProdB = glm::dot(dirB, planeFoward);

	if (dotProdA < 0.0f && dotProdB < 0.0f)
	{
		return false;
	}
	else {
		return true;
	}
}