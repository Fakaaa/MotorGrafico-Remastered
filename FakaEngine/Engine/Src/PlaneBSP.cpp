#include "PlaneBSP.h"
#include "Plane.h"

#include "Utils/Utils.h"

#pragma region CONSTRUCTORS

PlaneBSP::PlaneBSP()
{
}

PlaneBSP::~PlaneBSP()
{
	if (myPlane != NULL)
	{
		delete myPlane;
		myPlane = NULL;
	}
}

#pragma endregion

#pragma region PUBLIC_METHODS

void PlaneBSP::SetPlaneAttach(Entity* planeAttach)
{
	this->planeAttach = planeAttach;

	GenerateBspPlane();
}

void PlaneBSP::GenerateBspPlane()
{
	if (planeAttach != NULL)
	{
		glm::vec3 planeRotation = planeAttach->transform.forward;

		if (planeRotation == glm::vec3(90, 0, 0)) {

		}
	}
}

Entity* PlaneBSP::GetPlaneAttach()
{
	return planeAttach;
}

bool PlaneBSP::CheckObjectInPlane(Entity* objectCompare)
{
	return false;
}

bool PlaneBSP::CheckPositionInPlane(glm::vec3 position)
{
	return false;
}
#pragma endregion