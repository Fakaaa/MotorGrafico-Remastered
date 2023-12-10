#ifndef PLANEBSP_H
#define PLANEBSP_H

#include "Entity.h"

#include <string>
#include <string.h>

#include "Utils/Utils.h"
#include "PrivateClass/Export.h"

class MyPlane;

class ENGINE_API PlaneBSP
{
public:
	PlaneBSP();
	~PlaneBSP();

	vector<Entity*> ObjectsInGame;

	void SetPlaneAttach(Entity* plane);
	void GenerateBspPlane();

	Entity* GetPlaneAttach();

	void SetName(string value) { name = value; }
	string GetName() { return name; }

	bool CheckObjectInPlane(Entity* ObjectCompare);
	bool CheckPositionInPlane(glm::vec3 position);
private:

	string name;

	Entity* planeAttach = NULL;
	MyPlane* myPlane = NULL; // Con esto saco la matematica del plano
};

#endif