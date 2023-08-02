#ifndef FRUSTRUMCULLING_H
#define FRUSTRUMCULLING_H

#include <vector>
#include "Camera.h"
#include "Windows.h"
#include "PrivateClass/Export.h"
#include <iostream>

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

class MyPlane;
class Entity;
class ENGINE_API FrustrumCulling
{
public:
	FrustrumCulling();
	~FrustrumCulling();
	void SetDrawDistance(float value) { drawDistance = value; }
	float GetDrawDistance() { return drawDistance; }
	bool positiveNear(glm::vec3 point);
	bool positiveFar(glm::vec3 point);
	bool positiveLeft(glm::vec3 point);
	bool positiveRight(glm::vec3 point);
	bool positiveTop(glm::vec3 point);
	bool positiveDown(glm::vec3 point);
	void CheckObjectInFrustrum(int indexObject, vector<int>& indexsObjectsDisables, Entity* objectCompare);

	void UpdateFrustrum(Camera* _camera);
private:
	float drawDistance = 100;
	MyPlane* _nearPlane = NULL;
	MyPlane* _farPlane = NULL;
	MyPlane* _rightPlane = NULL;
	MyPlane* _leftPlane = NULL;
	MyPlane* _downPlane = NULL;
	MyPlane* _topPlane = NULL;
	bool CheckObjectInPlane(MyPlane* plane, Entity* objectCompare);
};
#endif