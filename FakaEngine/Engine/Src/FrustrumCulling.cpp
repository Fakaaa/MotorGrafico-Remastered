#include "FrustrumCulling.h";
#include "Plane.h"
#include "Entity.h"

void FrustrumCulling::UpdateFrustrum(Camera* camera)
{
	float offsetRightPlane = 125;
	float offsideNearValue = camera->projectionDataPerspective._near;
	float offsideFarPlane = camera->projectionDataPerspective._front;

	glm::vec4 cameraForward = camera->GetForward();
	glm::vec4 auxCameraForward = cameraForward;
	glm::vec4 cameraUp = camera->GetUp();
	glm::vec4 cameraRight = camera->GetRight();

	glm::vec4 cameraPosition = glm::vec4(camera->transform.position, 0);
	glm::vec4 offsideNearPlane = cameraForward + glm::vec4(0, 0, offsideNearValue, 0);

	//Calculo los planos Near y Far.
	_nearPlane->set3Points(cameraForward, cameraPosition + offsideNearPlane);
	_farPlane->set3Points(cameraForward, cameraPosition + offsideNearPlane + glm::vec4(0, 0, offsideFarPlane + drawDistance, 0));
	//================================//

	//Calculo los planos Right y Left.
	glm::mat4 rotCameraForward;

	rotCameraForward = glm::rotate(glm::mat4(1.0f), glm::radians(camera->projectionDataPerspective._FOV / 1.6f), glm::vec3(0, 1, 0));
	cameraForward = cameraForward * rotCameraForward;

	_leftPlane->set3Points(cameraForward, cameraPosition);
	cameraForward = auxCameraForward;

	rotCameraForward = glm::rotate(glm::mat4(1.0f), glm::radians(-camera->projectionDataPerspective._FOV / 1.5f), glm::vec3(0, 1, 0));
	cameraForward = cameraForward * rotCameraForward;

	_rightPlane->set3Points(cameraForward, glm::vec4(camera->transform.position.x + offsetRightPlane, camera->transform.position.y, camera->transform.position.z, 0));
	cameraForward = auxCameraForward;
	//================================//

	//Calculo los planos Top y Down.
	float angleRotate = camera->projectionDataPerspective._FOV - 10;

	rotCameraForward = glm::rotate(glm::mat4(1.0f), glm::radians(angleRotate), glm::vec3(1, 0, 0));
	cameraForward = cameraForward * rotCameraForward;

	_topPlane->set3Points(cameraForward, cameraPosition);
	cameraForward = auxCameraForward;

	rotCameraForward = glm::rotate(glm::mat4(1.0f), glm::radians(-angleRotate), glm::vec3(1, 0, 0));
	cameraForward = cameraForward * rotCameraForward;

	_downPlane->set3Points(cameraForward, cameraPosition);
	cameraForward = auxCameraForward;
	//================================//

	//Flipeo los planos
	_nearPlane->flipPlane();
	_rightPlane->flipPlane();
	_leftPlane->flipPlane();
	_downPlane->flipPlane();
	_topPlane->flipPlane();
	//========================//
}

void FrustrumCulling::CheckObjectInFrustrum(int indexObject, vector<int>& indexsObjectsDisables, Entity* objectCompare)
{
	bool objectInFrustrum = (CheckObjectInPlane(_nearPlane, objectCompare)
		&& CheckObjectInPlane(_farPlane, objectCompare)
		&& CheckObjectInPlane(_rightPlane, objectCompare)
		&& CheckObjectInPlane(_leftPlane, objectCompare)
		&& CheckObjectInPlane(_topPlane, objectCompare)
		&& CheckObjectInPlane(_downPlane, objectCompare));

	if (!objectInFrustrum)
	{
		indexsObjectsDisables.push_back(indexObject);
	}

}

bool FrustrumCulling::CheckObjectInPlane(MyPlane* plane, Entity* objectCompare)
{
	if (plane->getSide(objectCompare->GetAABBGlobalPositions()[0]) ||
		plane->getSide(objectCompare->GetAABBGlobalPositions()[1]) ||
		plane->getSide(objectCompare->GetAABBGlobalPositions()[2]) ||
		plane->getSide(objectCompare->GetAABBGlobalPositions()[3]) ||
		plane->getSide(objectCompare->GetAABBGlobalPositions()[4]) ||
		plane->getSide(objectCompare->GetAABBGlobalPositions()[5]) ||
		plane->getSide(objectCompare->GetAABBGlobalPositions()[6]) ||
		plane->getSide(objectCompare->GetAABBGlobalPositions()[7]))
	{
		return true;
	}
	return false;
}

FrustrumCulling::FrustrumCulling()
{
	_nearPlane = new MyPlane(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
	_farPlane = new MyPlane(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
	_rightPlane = new MyPlane(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
	_leftPlane = new MyPlane(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
	_downPlane = new MyPlane(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
	_topPlane = new MyPlane(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
}
FrustrumCulling::~FrustrumCulling()
{
	if (_nearPlane != NULL)
	{
		delete _nearPlane;
		_nearPlane = NULL;
	}
	if (_farPlane != NULL)
	{
		delete _farPlane;
		_farPlane = NULL;
	}
	if (_downPlane != NULL)
	{
		delete _downPlane;
		_downPlane = NULL;
	}
	if (_topPlane != NULL)
	{
		delete _topPlane;
		_topPlane = NULL;
	}
	if (_leftPlane != NULL)
	{
		delete _leftPlane;
		_leftPlane = NULL;
	}
	if (_rightPlane != NULL)
	{
		delete _rightPlane;
		_rightPlane = NULL;
	}
}

bool FrustrumCulling::positiveNear(glm::vec3 point)
{
	if (_nearPlane == NULL)
		return false;

	return _nearPlane->getSide(point);
}

bool FrustrumCulling::positiveFar(glm::vec3 point)
{
	if (_farPlane == NULL)
		return false;

	return _farPlane->getSide(point);
}

bool FrustrumCulling::positiveLeft(glm::vec3 point)
{
	if (_leftPlane == NULL)
		return false;

	return _leftPlane->getSide(point);
}

bool FrustrumCulling::positiveRight(glm::vec3 point)
{
	if (_rightPlane == NULL)
		return false;

	return _rightPlane->getSide(point);
}

bool FrustrumCulling::positiveTop(glm::vec3 point)
{
	if (_topPlane == NULL)
		return false;

	return _topPlane->getSide(point);
}

bool FrustrumCulling::positiveDown(glm::vec3 point)
{
	if (_downPlane == NULL)
		return false;

	return _downPlane->getSide(point);
}