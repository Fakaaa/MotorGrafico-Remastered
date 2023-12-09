#ifndef PLANE_H
#define PLANE_H

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

class MyPlane
{
public:
	MyPlane(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC);
	~MyPlane();

	glm::vec3 GetPlaneNormal() { return _normal; }

	bool GetSide(glm::vec3 point);
	void FlipPlane();
private:
	float GetDistanceToPoint(glm::vec3 point);

	glm::vec3 _normal;
	float _distance;
};
#endif // !PLANE_H