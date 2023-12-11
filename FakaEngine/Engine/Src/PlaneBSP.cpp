#include "PlaneBSP.h"
#include "Plane.h"
#include "Primitive3D.h"

#include "glm/vec3.hpp"
#include "Utils/Utils.h"

#include <math.h>

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
}

void PlaneBSP::SetupBspPlane(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, Renderer* renderer, DirectionCheck directionCheck)
{
	if (myPlane == NULL)
	{
		this->directionCheck = directionCheck;
		myPlane = new MyPlane(pointA, pointB, pointC);

		_shapeReference = new Primitive3D(renderer);

		_shapeReference->SetPosition(GetPlaneNormal());
		_shapeReference->SetScale(1, 1, 1);
		
		planeAttach->AddChildren(_shapeReference);
	}
}

bool PlaneBSP::ValidateObject(glm::vec3 meshMinColl, glm::vec3 meshMaxColl)
{
	if (myPlane != NULL)
	{
		return GetSide(meshMinColl) || GetSide(meshMaxColl);
	}

	return false;
}

Entity* PlaneBSP::GetPlaneAttach()
{
	return planeAttach;
}

void PlaneBSP::DrawShapeReference(bool &wireFrameActive)
{
	if (_shapeReference != NULL)
	{
		_shapeReference->Draw(wireFrameActive);
	}
}

void PlaneBSP::UpdateShapeReference()
{
	glm::vec3 shapeRefPosition = GetPlaneNormal();
	_shapeReference->SetPosition(shapeRefPosition);
}

float PlaneBSP::GetDistanceToPoint(glm::vec3 point)
{
	glm::vec3 dirFromAtoB = glm::normalize(point - planeAttach->transform.position);
	float dotProd = glm::dot(dirFromAtoB, GetPlaneNormal());

	cout << "Distance: " << dotProd << endl;

	return dotProd;
}

bool PlaneBSP::GetSide(glm::vec3 point)
{
	return GetDistanceToPoint(point) > 0.0f;
}

glm::vec3 PlaneBSP::GetPlaneNormal()
{
	glm::vec3 dirToCheck;

	switch (directionCheck)
	{
	case PlaneBSP::Forward:
		dirToCheck = glm::vec3(planeAttach->GetForward().x, planeAttach->GetForward().y, planeAttach->GetForward().z);
		break;
	case PlaneBSP::Back:
		dirToCheck = glm::vec3(-planeAttach->GetForward().x, -planeAttach->GetForward().y, -planeAttach->GetForward().z);
		break;
	case PlaneBSP::Left:
		dirToCheck = glm::vec3(-planeAttach->GetRight().x, -planeAttach->GetRight().y, -planeAttach->GetRight().z);
		break;
	case PlaneBSP::Right:
		dirToCheck = glm::vec3(planeAttach->GetRight().x, planeAttach->GetRight().y, planeAttach->GetRight().z);
		break;
	}

	return dirToCheck;
}
#pragma endregion