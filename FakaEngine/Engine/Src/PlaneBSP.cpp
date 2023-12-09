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
}
#pragma endregion

#pragma region PUBLIC_METHODS

void PlaneBSP::SetPlaneAttach(Entity* planeAttach)
{
	this->planeAttach = planeAttach;
}

void PlaneBSP::SetupBspPlane(Renderer* renderer, DirectionCheck directionCheck)
{
	this->directionCheck = directionCheck;

	_shapeReference = new Primitive3D(renderer);

	_shapeReference->SetPosition(GetPlaneNormal());
	_shapeReference->SetScale(0.25f, 0.25f, 0.25f);

	planeAttach->AddChildren(_shapeReference);
}

float PlaneBSP::GetDistanceToPoint(glm::vec3 point)
{
	glm::vec3 dirFromAtoB = glm::normalize(point - planeAttach->transform.position);
	float dotProd = glm::dot(dirFromAtoB, GetPlaneNormal());

	return dotProd;
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