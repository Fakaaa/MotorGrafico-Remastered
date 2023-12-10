#ifndef PLANE_H
#define PLANE_H

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

class MyPlane
{
public:
	MyPlane(glm::vec3 angleLockA, glm::vec3 angleLockB);
	~MyPlane();

	bool CheckObjectPass(glm::vec3 meshMinColl, glm::vec3 meshMaxColl, glm::vec3 position, glm::vec3 planeFoward);
private:

	glm::vec3 _angleLockA;
	glm::vec3 _angleLockB;
};
#endif // !PLANE_H