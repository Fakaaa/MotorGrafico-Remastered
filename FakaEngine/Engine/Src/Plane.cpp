#include "Plane.h"

#include <math.h>

MyPlane::MyPlane(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC)
{
	glm::vec3 line1 = pointB - pointA;
	glm::vec3 line2 = pointC - pointA;
	glm::vec3 rawNormal = glm::cross(line1, line2);

	_normal = glm::normalize(rawNormal);
	_distance = 0 - glm::dot(_normal, pointA);
}

MyPlane::~MyPlane() {}

void MyPlane::FlipPlane()
{
	_normal = -_normal;
	_distance = -_distance;
}

float MyPlane::GetDistanceToPoint(glm::vec3 point)
{
	return glm::dot(_normal, point) + _distance;
}

bool MyPlane::GetSide(glm::vec3 point) 
{
	return GetDistanceToPoint(point) > 0.0f;
}