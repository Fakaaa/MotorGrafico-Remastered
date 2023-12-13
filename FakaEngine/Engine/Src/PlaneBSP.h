#ifndef PLANEBSP_H
#define PLANEBSP_H

#include "Entity.h"

#include <string>
#include <string.h>

#include "Utils/Utils.h"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

#include "Primitive3D.h"

#include "PrivateClass/Export.h"

class MyPlane;

class ENGINE_API PlaneBSP
{
public:
	PlaneBSP();
	~PlaneBSP();

	enum DirectionCheck
	{
		Forward,
		Back,
		Left,
		Right
	};

	vector<Entity*> ObjectsInGame;

	void SetPlaneAttach(Entity* plane);
	void SetupBspPlane(Renderer* renderer, DirectionCheck directionCheck);

	Entity* GetPlaneAttach();

	void DrawShapeReference(bool& wireFrameActive);
	void UpdateShapeReference();

	float GetDistanceToPoint(glm::vec3 point);

	void SetName(string value) { name = value; }
	string GetName() { return name; }
private:

	glm::vec3 GetPlaneNormal();

	string name;

	DirectionCheck directionCheck;
	Primitive3D* _shapeReference = NULL;

	Entity* planeAttach = NULL;
};

#endif