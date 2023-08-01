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

	struct VertexsPlaneBSP {

	public:
		float vertex_max_x;
		float vertex_min_x;
		float vertex_max_y;
		float vertex_min_y;
		float vertex_max_z;
		float vertex_min_z;
		VertexsPlaneBSP(float max_x, float min_x, float max_y, float min_y, float max_z, float min_z)
		{
			vertex_max_x = max_x;
			vertex_min_x = min_x;
			vertex_max_y = max_y;
			vertex_min_y = min_y;
			vertex_max_z = max_z;
			vertex_min_z = min_z;
		}
	};

	enum CurrentCameraPosition
	{
		Null,
		PostivePlane,
		NegativePlane,
	};

	CurrentCameraPosition currentCameraPosition = Null;
	vector<Entity*> ObjectsInGame;
	VertexsPlaneBSP vertexPlaneBSP = VertexsPlaneBSP(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	void SetPlaneAttach(Entity* plane);
	void SetCurrentCameraCompare(Entity* camera);
	Entity* GetPlaneAttach();
	void GeneratePlane();
	void SetName(string value) { name = value; }
	string GetName() { return name; }
	void CheckObjectInPlaneBSP(Entity* ObjectCompare, int indexPlanePush, vector<int>& indexsDisableObjects);
private:

	Entity* currentCameraCompare;
	string name;

	Entity* planeAttach = NULL;
	MyPlane* myPlane = NULL; // Con esto saco la matematica del plano
};

#endif