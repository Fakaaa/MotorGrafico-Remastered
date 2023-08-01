#include "Camera.h"
#include <glew.h>
#include <GLFW/glfw3.h>

#include "Material.h"
#include "AxisAlignedBoundingBox.h"
#include "FrustrumCulling.h"
#include "CollisionManager.h"
#include "Window.h"

Camera::Camera(Renderer* _render, TypeProjectionCamera _typeProjectionCamera) : Entity(_render)
{
	_MVP.view = glm::mat4(1.0f);
	_MVP.projection = glm::mat4(1.0f);
	typeProjectionCamera = _typeProjectionCamera;
	typeCamera = FirstPerson;
	frustrumCulling = new FrustrumCulling();
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

	if (frustrumCulling != NULL) {
		delete frustrumCulling;
		frustrumCulling = NULL;
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

void Camera::SetIndexBSPPlanes(int plane1, int plane2, int plane3)
{
	indicesBSP[0] = plane1;
	indicesBSP[1] = plane2;
	indicesBSP[2] = plane3;
}

void Camera::SetEnableDrawAABB(bool value)
{
	if (axisAlignedBoundingBox != NULL)
		axisAlignedBoundingBox->SetEnableDraw(value);

	if (_AABBOrthographic != NULL)
		_AABBOrthographic->SetEnableDraw(value);

	if (_AABBPerspective != NULL)
		_AABBPerspective->SetEnableDraw(value);
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


void Camera::disableChildrenAndParent(Entity* parent)
{
	parent->SetIsAlive(false);
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

glm::vec3 Camera::GetFrontView()
{
	return _front;
}

glm::vec3 Camera::GetRightView()
{
	return _right;
}

glm::vec3 Camera::GetUpView()
{
	return _up;
}

string Camera::GetNameOfClass()
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

void Camera::UseFrustrum(float aspectRatio)
{
	if (useFrustrum)
	{
		for (int i = 0; i < objectsCheckFrustrum.size(); i++)
		{
			objectsCheckFrustrum[i]->SetIsAlive(true);
		}
		frustrumCulling->UpdateFrustrum(this);

		for (int i = 0; i < objectsCheckFrustrum.size(); i++)
		{
			if (objectsCheckFrustrum[i]->GetIsAlive())
				objectsCheckFrustrum[i]->CheckVisibleFrustrumCulling(objectsCheckFrustrum, indexsObjectsDisable, frustrumCulling);
		}

		for (int i = 0; i < indexsObjectsDisable.size(); i++)
		{
			objectsCheckFrustrum[indexsObjectsDisable[i]]->SetIsAlive(false);
		}

		indexsObjectsDisable.clear();
	}
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
