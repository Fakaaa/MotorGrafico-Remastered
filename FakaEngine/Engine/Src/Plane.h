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
	MyPlane(glm::vec3 normal, glm::vec3 point);
	~MyPlane();

	void set3Points(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC);

	glm::vec3 getNormal() { return glm::normalize(_normal); }

	void flipPlane();

	float getDistanceToPoint(glm::vec3 point);
	bool getSide(glm::vec3 point);

private:
	float _distance;
	glm::vec3 _normal;
};
#endif // !PLANE_H