#include "PlaneBSP.h"
#include "Plane.h"

#pragma region CONSTRUCTORS

PlaneBSP::PlaneBSP()
{
	currentCameraPosition = ObjectPlanePosition::Null;
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

void PlaneBSP::SetCurrentCameraCompare(Entity* currentCameraCompare)
{
	this->currentCameraCompare = currentCameraCompare;
}

void PlaneBSP::GenerateBspPlane()
{
	if (planeAttach != NULL)
	{
		glm::vec3 PointA = glm::vec3(0.0f);
		glm::vec3 PointB = glm::vec3(0.0f);
		glm::vec3 PointC = glm::vec3(0.0f);

		if ((int)vertexPlaneBSP.vertex_min_x == (int)vertexPlaneBSP.vertex_max_x)
		{
			PointA = glm::vec3(planeAttach->transform.position.x
				, planeAttach->transform.position.y + (planeAttach->transform.scale.y)
				, planeAttach->transform.position.z);

			PointB = glm::vec3(planeAttach->transform.position.x
				, planeAttach->transform.position.y
				, planeAttach->transform.position.z + (planeAttach->transform.scale.z));

			PointC = glm::vec3(planeAttach->transform.position.x
				, planeAttach->transform.position.y
				, planeAttach->transform.position.z);
		}
		else if ((int)vertexPlaneBSP.vertex_min_y == (int)vertexPlaneBSP.vertex_max_y)
		{
			PointA = glm::vec3(planeAttach->transform.position.x + (planeAttach->transform.scale.x)
				, planeAttach->transform.position.y
				, planeAttach->transform.position.z);

			PointB = glm::vec3(planeAttach->transform.position.x
				, planeAttach->transform.position.y + (planeAttach->transform.scale.y)
				, planeAttach->transform.position.z);

			PointC = glm::vec3(planeAttach->transform.position.x
				, planeAttach->transform.position.y
				, planeAttach->transform.position.z);
		}
		else if ((int)vertexPlaneBSP.vertex_min_z == (int)vertexPlaneBSP.vertex_max_z)
		{
			//cout << "ENTRE AL PLANO EJE Y" << endl;
			PointA = glm::vec3(planeAttach->transform.position.x + (planeAttach->transform.scale.x)
				, planeAttach->transform.position.y
				, planeAttach->transform.position.z);

			PointB = glm::vec3(planeAttach->transform.position.x
				, planeAttach->transform.position.y
				, planeAttach->transform.position.z + (planeAttach->transform.scale.z));

			PointC = glm::vec3(planeAttach->transform.position.x
				, planeAttach->transform.position.y
				, planeAttach->transform.position.z);
		}

		myPlane = new MyPlane(PointA, PointB, PointC);
	}
}

Entity* PlaneBSP::GetPlaneAttach()
{
	return planeAttach;
}

void PlaneBSP::SetVerticesBSP(VerticesBSP vertices)
{
	vertexPlaneBSP.vertex_max_x = vertices.vertex_max_x;
	vertexPlaneBSP.vertex_min_x = vertices.vertex_min_x;
	vertexPlaneBSP.vertex_max_y = vertices.vertex_max_y;
	vertexPlaneBSP.vertex_min_y = vertices.vertex_min_y;
	vertexPlaneBSP.vertex_max_z = vertices.vertex_max_z;
	vertexPlaneBSP.vertex_min_z = vertices.vertex_min_z;
}

bool PlaneBSP::CheckObjectInPlaneBSP(Entity* objectCompare)
{
	ObjectPlanePosition objectToComparePosition = ObjectPlanePosition::Null;

	//Validate the current camera position along this BSP_Plane.
	if (myPlane->getSide(currentCameraCompare->transform.position))
	{
		currentCameraPosition = ObjectPlanePosition::PostivePlane;
	}
	else 
	{
		currentCameraPosition = ObjectPlanePosition::NegativePlane;
	}

	//Validate the current object to compare along this BSP_Plane.
	if (myPlane->getSide(objectCompare->GetAABBGlobalPositions()[0]) && myPlane->getSide(objectCompare->GetAABBGlobalPositions()[1]) &&
		myPlane->getSide(objectCompare->GetAABBGlobalPositions()[2]) && myPlane->getSide(objectCompare->GetAABBGlobalPositions()[3]) &&
		myPlane->getSide(objectCompare->GetAABBGlobalPositions()[4]) && myPlane->getSide(objectCompare->GetAABBGlobalPositions()[5]) &&
		myPlane->getSide(objectCompare->GetAABBGlobalPositions()[6]) && myPlane->getSide(objectCompare->GetAABBGlobalPositions()[7]))
	{
		objectToComparePosition = ObjectPlanePosition::PostivePlane;
	}
	else
	{
		objectToComparePosition = ObjectPlanePosition::NegativePlane;
	}

	//Log the result
	/*switch (currentCameraPosition)
	{
	case PlaneBSP::Null:
		cout << "The camera has the enum in NULL state" << endl;
		break;
	case PlaneBSP::PostivePlane:
		cout << "Camera is on the POSITIVE side of the plane: " << name << endl;
		break;
	case PlaneBSP::NegativePlane:
		cout << "Camera is on the NEGATIVE side of the plane: " << name << endl;
		break;
	}*/

	//Return true if the item to compare is along the same side that the camera
	return objectToComparePosition == currentCameraPosition;
}

#pragma endregion