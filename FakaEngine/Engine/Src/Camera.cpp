#include "Camera.h"
#include <glew.h>
#include <GLFW/glfw3.h>
#include "Material.h"
#include "AxisAlignedBoundingBox.h"

#include "CollisionManager.h"

Camera::Camera(Renderer* _render, TypeProjectionCamera _typeProjectionCamera) : Entity(_render)
{
	_MVP.view = glm::mat4(1.0f);
	_MVP.projection = glm::mat4(1.0f);
	typeProjectionCamera = _typeProjectionCamera;
	typeCamera = FirstPerson;
	InmortalObject = true;
}

Camera::~Camera()
{
	if (_AABBOrthographic != NULL && _AABBPerspective != NULL)
	{
		delete _AABBOrthographic;
		delete _AABBPerspective;
		_AABBOrthographic = NULL;
		_AABBPerspective = NULL;
	}

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

void Camera::ChangeActualFrustrum()
{
	switch (typeProjectionCamera)
	{
	case Perspective:
		_actualFrustrumInUse = _AABBPerspective;
		cout << "CHANGED TO PERSPECTIVE" << endl;
		break;
	case Ortho:
		_actualFrustrumInUse = _AABBOrthographic;
		cout << "CHANGED TO ORTHOGRAPHIC" << endl;
		break;
	}
}

bool Camera::positiveNear(glm::vec3 point)
{
	if (_nearPlane == NULL)
		return false;

	return _nearPlane->getSide(point);
}

bool Camera::positiveFar(glm::vec3 point)
{
	if (_farPlane == NULL)
		return false;

	return _farPlane->getSide(point);
}

bool Camera::positiveLeft(glm::vec3 point)
{
	if (_leftPlane == NULL)
		return false;

	return _leftPlane->getSide(point);
}

bool Camera::positiveRight(glm::vec3 point)
{
	if (_rightPlane == NULL)
		return false;

	return _rightPlane->getSide(point);
}

bool Camera::positiveTop(glm::vec3 point)
{
	if (_topPlane == NULL)
		return false;

	return _topPlane->getSide(point);
}

bool Camera::positiveDown(glm::vec3 point)
{
	if (_downPlane == NULL)
		return false;

	return _downPlane->getSide(point);
}

void Camera::updateFrustrumPlanes()
{
	if (_farPlane != NULL && _nearPlane != NULL && _topPlane != NULL
		&& _downPlane != NULL && _leftPlane != NULL && _rightPlane != NULL)
	{
		glm::vec3 point0 = glm::vec3(transform.position.x + _AABBPerspective->GetAABBPositions()[0].x, transform.position.y + _AABBPerspective->GetAABBPositions()[0].y,
			transform.position.z + _AABBPerspective->GetAABBPositions()[0].z);

		glm::vec3 point1 = glm::vec3(transform.position.x + _AABBPerspective->GetAABBPositions()[1].x, transform.position.y + _AABBPerspective->GetAABBPositions()[1].y,
			transform.position.z + _AABBPerspective->GetAABBPositions()[1].z);

		glm::vec3 point2 = glm::vec3(transform.position.x + _AABBPerspective->GetAABBPositions()[2].x, transform.position.y + _AABBPerspective->GetAABBPositions()[2].y,
			transform.position.z + _AABBPerspective->GetAABBPositions()[2].z);

		glm::vec3 point3 = glm::vec3(transform.position.x + _AABBPerspective->GetAABBPositions()[3].x, transform.position.y + _AABBPerspective->GetAABBPositions()[3].y,
			transform.position.z + _AABBPerspective->GetAABBPositions()[3].z);

		glm::vec3 point4 = glm::vec3(transform.position.x + _AABBPerspective->GetAABBPositions()[4].x, transform.position.y + _AABBPerspective->GetAABBPositions()[4].y,
			transform.position.z + _AABBPerspective->GetAABBPositions()[4].z);

		glm::vec3 point5 = glm::vec3(transform.position.x + _AABBPerspective->GetAABBPositions()[5].x, transform.position.y + _AABBPerspective->GetAABBPositions()[5].y,
			transform.position.z + _AABBPerspective->GetAABBPositions()[5].z);

		glm::vec3 point6 = glm::vec3(transform.position.x + _AABBPerspective->GetAABBPositions()[6].x, transform.position.y + _AABBPerspective->GetAABBPositions()[6].y,
			transform.position.z + _AABBPerspective->GetAABBPositions()[6].z);

		glm::vec3 point7 = glm::vec3(transform.position.x + _AABBPerspective->GetAABBPositions()[7].x, transform.position.y + _AABBPerspective->GetAABBPositions()[7].y,
			transform.position.z + _AABBPerspective->GetAABBPositions()[7].z);

		if (_AABBPerspective != NULL)
		{
			_nearPlane->set3Points(point0, point3, point2);

			_farPlane->set3Points(point4, point7, point6);

			_rightPlane->set3Points(point7, point6, point2);

			_leftPlane->set3Points(point4, point5, point1);

			_downPlane->set3Points(point5, point6, point2);

			_topPlane->set3Points(point7, point4, point0);

			_rightPlane->flipPlane();
			_nearPlane->flipPlane();
		}
	}
}

void Camera::SetIndexBSPPlanes(int plane1, int plane2, int plane3)
{
	indicesBSP[0] = plane1;
	indicesBSP[1] = plane2;
	indicesBSP[2] = plane3;
}

void Camera::renderThingsOnScene(CollisionManager* frustrumCheck, vector<Entity*> objectsInScene)
{
	for (int i = 0; i < objectsInScene.size(); i++)
	{
		checkObjectHerarchy(frustrumCheck, objectsInScene[i]);
	}
}

void Camera::SetEnableDrawAABB(bool value)
{
	if (axisAlignedBoundingBox != NULL)
		axisAlignedBoundingBox->SetEnableDraw(value);
}

void Camera::BindBuffer() {}

void Camera::Draw(bool& wireFrameActive)
{
	UseCamera(renderer->GetCurrentShaderUse(), internalData.localModel);
}
void Camera::UseCamera(Shader& shader, glm::mat4 trsCamera)
{
	unsigned int transformLoc = glGetUniformLocation(shader.getId(), "model");
	unsigned int projectionLoc = glGetUniformLocation(shader.getId(), "projection");
	unsigned int viewLoc = glGetUniformLocation(shader.getId(), "view");
	glUseProgram(shader.getId());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trsCamera));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(_MVP.projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(_MVP.view));

	switch (typeProjectionCamera)
	{
	case Perspective:
		if (_AABBPerspective != NULL)
		{
			_AABBPerspective->UpdateInternalDataBoundingBox(internalData, transform);
			//_AABBPerspective->Draw(_AABBPerspective->GetEnableDraw());
			bool disable = false;
			_AABBPerspective->Draw(disable);
		}
		break;
	case Ortho:
		if (_AABBOrthographic != NULL)
		{
			_AABBOrthographic->UpdateInternalDataBoundingBox(internalData, transform);
			_AABBOrthographic->Draw(_AABBOrthographic->GetEnableDraw());
		}
		break;
	}
}
void Camera::SetViewFirstPerson()
{
	_MVP.view = CalculateViewMatrix();
}

void Camera::SetViewThirdPerson()
{
	_MVP.view = glm::lookAt(transform.position, targetThirdPerson->transform.position + _front, _up);
	CalculateThirdPersonPositionCamera();
}

void Camera::CalculateThirdPersonPositionCamera()
{
	float theta = targetThirdPerson->transform.rotation.y;
	float posX = CalculateHorizontalDistanceOfTarget() * glm::sin(glm::radians(theta));
	float posY = targetThirdPerson->transform.position.y + CalculateVerticalDistanceOfTarget();
	float posZ = CalculateHorizontalDistanceOfTarget() * glm::cos(glm::radians(theta));

	float finalPosX = targetThirdPerson->transform.position.x - posX;
	float finalPosY = posY;
	float finalPosZ = targetThirdPerson->transform.position.z - posZ;

	_yaw = 180 - glm::radians(targetThirdPerson->transform.rotation.y);

	SetPosition(finalPosX, finalPosY, finalPosZ);

	float _calcDistanceZ = CalculateDistanceOfTarget() - initOffsetCameraThirdPersonZ;

	glm::vec3 vec = glm::vec3(initOffsetCameraThirdPersonX, initOffsetCameraThirdPersonY, targetThirdPerson->GetForward().z * _calcDistanceZ);
	glm::vec3 addVector = transform.position + vec;
	SetPosition(addVector);


	lastPositionTarget = targetThirdPerson->transform.position;

	UpdateCamera();
}

void Camera::checkObjectHerarchy(CollisionManager* frustrumCheck, Entity* object)
{
	if (object->GetIsInmortal())
		return;

	if (!frustrumCheck->CheckEntitiesOnFrustrum(this, object))
		disableChildrenAndParent(object);
	else {
		object->SetIsAlive(true);
		for (int i = 0; i < object->GetChildrens().size(); i++)
		{
			checkObjectHerarchy(frustrumCheck, object->GetChildrens()[i]);
		}
	}
}

void Camera::disableChildrenAndParent(Entity* parent)
{
	parent->SetIsAlive(false);
	//for (Entity* child : parent->GetChildrens())
	//{
	//	disableChildrenAndParent(child);
	//}
}

void Camera::enableChildrenAndParent(Entity* parent)
{
	parent->SetIsAlive(true);
	for (Entity* child : parent->GetChildrens())
	{
		enableChildrenAndParent(child);
	}
}

float Camera::CalculateVerticalDistanceOfTarget()
{
	return CalculateDistanceOfTarget() * glm::sin(glm::radians(_pitch));
}

float Camera::CalculateDistanceOfTarget()
{
	return glm::distance(transform.position, targetThirdPerson->transform.position);
}

float Camera::CalculateHorizontalDistanceOfTarget()
{
	return CalculateDistanceOfTarget() * glm::cos(glm::radians(_pitch));
}

void Camera::SetProjectionPerspective(float FOV, float aspect, float _near, float front)
{
	//                                      FOV          Aspect  near   front
	_MVP.projection = glm::perspective(glm::radians(FOV), aspect, _near, front);
}

void Camera::SetProjectionOrtho(float left, float right, float bottom, float top, float _near, float front)
{
	_MVP.projection = glm::ortho(left, right, bottom, top, _near, front);
}

void Camera::InitCamera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch) {
	SetPosition(pos.x, pos.y, pos.z);
	_worldUp = up;
	_yaw = yaw;
	_pitch = pitch;
	_front = glm::vec3(0.0f, 0.0f, -1.0f);

	UpdateCamera();
}
glm::mat4 Camera::CalculateViewMatrix() {
	return glm::lookAt(transform.position, transform.position + _front, _up);
}
void Camera::UpdateCamera() {
	CheckIsModel();
	_front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front.y = sin(glm::radians(_pitch));
	_front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(_front);

	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));

	//CheckIsModel();
}
void Camera::SetPitch(float p) {
	_pitch = p;
	UpdateCamera();
}
void Camera::SetYaw(float y) {
	_yaw = y;
	UpdateCamera();
}
float Camera::GetPitch() {
	return _pitch;
}
float Camera::GetYaw() {
	return _yaw;
}

string Camera::GetClassName()
{
	return "Camera";
}

void Camera::SetDataPerspective(float FOV, float sizeScreenX, float sizeScreenY, float _near, float front)
{
	projectionDataPerspective._FOV = FOV;
	projectionDataPerspective._aspect = sizeScreenX / sizeScreenY;
	projectionDataPerspective._near = _near;
	projectionDataPerspective._front = front;
}

void Camera::SetDataOrtho(float left, float right, float bottom, float top, float _near, float front)
{
	projectionDataOrtho._left = left;
	projectionDataOrtho._right = right;
	projectionDataOrtho._bottom = bottom;
	projectionDataOrtho._top = top;
	projectionDataOrtho._near = _near;
	projectionDataOrtho._front = front;
}

void Camera::ChangePerspective(TypeProjectionCamera _typeProjectionCamera)
{
	typeProjectionCamera = _typeProjectionCamera;
	ChangeActualFrustrum();
	UseProjection();
}

void Camera::SetFrustrumCulling()
{
	_AABBOrthographic = new AxisAlignedBoundingBox(renderer);
	_AABBPerspective = new AxisAlignedBoundingBox(renderer);

	float nearDist = ((transform.position.z - projectionDataPerspective._near) / (projectionDataPerspective._FOV / projectionDataPerspective._aspect));
	float farDist = ((transform.position.z - projectionDataPerspective._front) / (projectionDataPerspective._FOV / projectionDataPerspective._aspect));

	float nearZ = (transform.position.z);
	float farZ = (projectionDataPerspective._front - transform.position.z);

	vector<glm::vec3> _dataXYZOrtho;
	vector<glm::vec3> _dataXYZPerspective;

	//float nearMinX = -100;
	//float nearMaxX =  400;

	float nearMinX = transform.position.x * (-nearDist);
	float nearMaxX = transform.position.x * (nearDist);
	float nearMinY = transform.position.y * (-nearDist);
	float nearMaxY = transform.position.y * (nearDist);

	float farMinX = transform.position.x * (-(farDist));
	float farMaxX = transform.position.x * ((farDist));
	float farMinY = transform.position.y * (-(farDist));
	float farMaxY = transform.position.y * ((farDist));

	cout << "NEAR DIST: " << nearDist << endl;
	cout << "FAR DIST: " << farDist << endl;

#pragma region ORTHOGRAPHIC VIEW 
	//--------------------------------------------------------------------
	//0
	_dataXYZOrtho.push_back(glm::vec3(projectionDataOrtho._left, projectionDataOrtho._top, projectionDataOrtho._front));
	//1
	_dataXYZOrtho.push_back(glm::vec3(projectionDataOrtho._left, projectionDataOrtho._bottom, projectionDataOrtho._front));
	//2
	_dataXYZOrtho.push_back(glm::vec3(projectionDataOrtho._right, projectionDataOrtho._bottom, projectionDataOrtho._front));
	//3
	_dataXYZOrtho.push_back(glm::vec3(projectionDataOrtho._right, projectionDataOrtho._top, projectionDataOrtho._front));

	//4
	_dataXYZOrtho.push_back(glm::vec3(projectionDataOrtho._left, projectionDataOrtho._top, projectionDataOrtho._near));
	//5
	_dataXYZOrtho.push_back(glm::vec3(projectionDataOrtho._left, projectionDataOrtho._bottom, projectionDataOrtho._near));
	//6
	_dataXYZOrtho.push_back(glm::vec3(projectionDataOrtho._right, projectionDataOrtho._bottom, projectionDataOrtho._near));
	//7
	_dataXYZOrtho.push_back(glm::vec3(projectionDataOrtho._right, projectionDataOrtho._top, projectionDataOrtho._near));

	_AABBOrthographic->SetVerticesColliders(_AABBOrthographic->GenerateAxisAlignedBoundingBoxPos(_dataXYZOrtho),
		_AABBOrthographic->GenerateAxisAlignedBoundingBoxCol());

	//-
	//--------------------------------------------------------------------
#pragma endregion

#pragma region PERSPECTIVE VIEW

//0
	_dataXYZPerspective.push_back(glm::vec3(nearMinX / 16, nearMaxY / 16, nearZ / 1.005f));
	//1																  
	_dataXYZPerspective.push_back(glm::vec3(nearMinX / 16, nearMinY / 16, nearZ / 1.005));
	//2																  
	_dataXYZPerspective.push_back(glm::vec3(nearMaxX / 16, nearMinY / 16, nearZ / 1.005));
	//3																  
	_dataXYZPerspective.push_back(glm::vec3(nearMaxX / 16, nearMaxY / 16, nearZ / 1.005));
	//4
	_dataXYZPerspective.push_back(glm::vec3(farMinX / 16, farMaxY / 16, farZ / 2));
	//5
	_dataXYZPerspective.push_back(glm::vec3(farMinX / 16, farMinY / 16, farZ / 2));
	//6
	_dataXYZPerspective.push_back(glm::vec3(farMaxX / 16, farMinY / 16, farZ / 2));
	//7
	_dataXYZPerspective.push_back(glm::vec3(farMaxX / 16, farMaxY / 16, farZ / 2));

	cout << "DATA PERSPECTIVE FRUSTRUM" << endl;
	for (int i = 0; i < _dataXYZPerspective.size(); i++)
	{
		cout << i << "_[" << _dataXYZPerspective[i].x << "][" << _dataXYZPerspective[i].y << "][" << _dataXYZPerspective[i].z << "]." << endl;
	}

	_nearPlane = new MyPlane(_dataXYZPerspective[0], _dataXYZPerspective[3], _dataXYZPerspective[2]);

	_farPlane = new MyPlane(_dataXYZPerspective[4], _dataXYZPerspective[7], _dataXYZPerspective[6]);

	_rightPlane = new MyPlane(_dataXYZPerspective[7], _dataXYZPerspective[6], _dataXYZPerspective[2]);

	_leftPlane = new MyPlane(_dataXYZPerspective[4], _dataXYZPerspective[5], _dataXYZPerspective[1]);

	_downPlane = new MyPlane(_dataXYZPerspective[5], _dataXYZPerspective[6], _dataXYZPerspective[2]);

	_topPlane = new MyPlane(_dataXYZPerspective[7], _dataXYZPerspective[4], _dataXYZPerspective[0]);

	_rightPlane->flipPlane();
	_nearPlane->flipPlane();

	_AABBPerspective->SetVerticesColliders(_AABBPerspective->GenerateAABBFrustrumPerspective(_dataXYZPerspective),
		_AABBPerspective->GenerateAxisAlignedBoundingBoxCol());
#pragma endregion

	_AABBOrthographic->AttachEntity(internalData, transform);
	_AABBPerspective->AttachEntity(internalData, transform);
}

void Camera::UseProjection()
{
	switch (typeProjectionCamera)
	{
	case Perspective:
		SetProjectionPerspective(
			projectionDataPerspective._FOV,
			projectionDataPerspective._aspect,
			projectionDataPerspective._near,
			projectionDataPerspective._front
		);
		break;
	case Ortho:
		SetProjectionOrtho(
			projectionDataOrtho._left,
			projectionDataOrtho._right,
			projectionDataOrtho._bottom,
			projectionDataOrtho._top,
			projectionDataOrtho._near,
			projectionDataOrtho._front
		);
		break;
	}
}

glm::mat4 Camera::getViewMat()
{
	return _MVP.view;
}
glm::mat4 Camera::getProjMat()
{
	return _MVP.projection;
}

void Camera::SetRotationY(float rotY)
{
	_yaw = rotY;
	UpdateCamera();
	Entity::SetRotationY(rotY);
}

void Camera::SetRotationX(float rotX)
{
	_pitch = rotX;
	UpdateCamera();
	Entity::SetRotationX(rotX);
}

void Camera::RotateCameraX(float speed)
{
	_pitch = _pitch + speed;
	UpdateCamera();
	Entity::SetRotationX(_pitch);
}

void Camera::RotateCameraY(float speed)
{
	_yaw = _yaw + speed;
	UpdateCamera();
	Entity::SetRotationY(_yaw);
}

void Camera::RotateCameraZ(float speed)
{
}
