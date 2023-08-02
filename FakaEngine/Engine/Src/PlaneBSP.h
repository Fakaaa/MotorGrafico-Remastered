#ifndef PLANEBSP_H
#define PLANEBSP_H

#include "Entity.h"

#include <string>
#include <string.h>

#include "PrivateClass/Export.h"

class MyPlane;

class ENGINE_API PlaneBSP
{
public:
	PlaneBSP();
	~PlaneBSP();

	struct VerticesBSP 
	{
		public:
		float vertex_max_x;
		float vertex_min_x;
		float vertex_max_y;
		float vertex_min_y;
		float vertex_max_z;
		float vertex_min_z;
		VerticesBSP(float max_x, float min_x, float max_y, float min_y, float max_z, float min_z)
		{
			vertex_max_x = max_x;
			vertex_min_x = min_x;
			vertex_max_y = max_y;
			vertex_min_y = min_y;
			vertex_max_z = max_z;
			vertex_min_z = min_z;
		}
	};

	enum ObjectPlanePosition
	{
		Null,
		PostivePlane,
		NegativePlane,
	};

	VerticesBSP vertexPlaneBSP = VerticesBSP(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	ObjectPlanePosition currentCameraPosition = Null;
	vector<Entity*> ObjectsInGame;

	void SetPlaneAttach(Entity* plane);
	void SetCurrentCameraCompare(Entity* _camera);
	void GenerateBspPlane();

	Entity* GetPlaneAttach();

	void SetName(string value) { name = value; }
	string GetName() { return name; }

	void SetVerticesBSP(VerticesBSP vertices);

	bool CheckObjectInPlaneBSP(Entity* ObjectCompare);
private:

	Entity* currentCameraCompare;
	string name;

	Entity* planeAttach = NULL;
	MyPlane* myPlane = NULL; // Con esto saco la matematica del plano
};

#endif