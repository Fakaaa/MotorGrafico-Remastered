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
	void SetupBspPlane(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, Renderer* renderer, DirectionCheck directionCheck);
	bool ValidateObject(glm::vec3 meshMinColl, glm::vec3 meshMaxColl);

	Entity* GetPlaneAttach();

	void DrawShapeReference(bool& wireFrameActive);
	void UpdateShapeReference();

	float GetDistanceToPoint(glm::vec3 point);
	bool GetSide(glm::vec3 point);

	MyPlane* GetLogicPlane() { return myPlane; }
	void SetName(string value) { name = value; }
	string GetName() { return name; }
private:

	glm::vec3 GetPlaneNormal();

	string name;

	DirectionCheck directionCheck;
	Primitive3D* _shapeReference = NULL;

	Entity* planeAttach = NULL;
	MyPlane* myPlane = NULL; // Con esto saco la matematica del plano
};

#endif