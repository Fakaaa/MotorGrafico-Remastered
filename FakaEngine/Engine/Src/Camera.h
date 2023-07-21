#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"
#include "Plane.h"

static enum TypeProjectionCamera
{
	Perspective,
	Ortho,
};
static enum TypeCamera
{
	FirstPerson,
	ThirdPerson,
};
static struct ProjectionDataOrtho
{
	float _left;
	float _right;
	float _bottom;
	float _top;
	float _near;
	float _front;
};

struct ProjectionDataPerspective
{
public:
	float _FOV;
	float _aspect;
	float _near;
	float _front;
};

class CollisionManager;

class ENGINE_API Camera : public Entity
{
private:

	TypeCamera typeCamera;
	Entity* targetThirdPerson;

	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;
	matrixMVP _MVP;

	AxisAlignedBoundingBox* _AABBOrthographic;
	AxisAlignedBoundingBox* _AABBPerspective;

	AxisAlignedBoundingBox* _actualFrustrumInUse;

	MyPlane* _nearPlane = NULL;
	MyPlane* _farPlane = NULL;
	MyPlane* _rightPlane = NULL;
	MyPlane* _leftPlane = NULL;
	MyPlane* _downPlane = NULL;
	MyPlane* _topPlane = NULL;


	float _yaw;
	float _pitch;
protected:
	float initOffsetCameraThirdPersonX = 0.0f;
	float initOffsetCameraThirdPersonY = 0.0f;
	float initOffsetCameraThirdPersonZ = 1.6f;
	glm::vec3 lastPositionTarget = glm::vec3(0.0f);
	float lastDistance = 0.0f;

	void SetEnableDrawAABB(bool value) override;
	void BindBuffer() override;

	void CalculateThirdPersonPositionCamera();

	void checkObjectHerarchy(CollisionManager* frustrumCheck, Entity* object);
	void disableChildrenAndParent(Entity* parent);
	void enableChildrenAndParent(Entity* parent);

	float offsetThirdPersonY = 350;

public:
	void ChangeActualFrustrum();

	bool positiveNear(glm::vec3 point);
	bool positiveFar(glm::vec3 point);
	bool positiveLeft(glm::vec3 point);
	bool positiveRight(glm::vec3 point);
	bool positiveTop(glm::vec3 point);
	bool positiveDown(glm::vec3 point);

	void updateFrustrumPlanes();

	int indicesBSP[amountPlanesBSP];

	void SetIndexBSPPlanes(int plane1, int plane2, int plane3);
	int* GetIndicesBSP() { return indicesBSP; }

	void renderThingsOnScene(CollisionManager* frustrumCheck, vector<Entity*> objectsInScene);

	void SetInitOffsetCameraThirdPersonX(float value) { initOffsetCameraThirdPersonX = value; }
	void SetInitOffsetCameraThirdPersonY(float value) { initOffsetCameraThirdPersonY = value; }
	void SetInitOffsetCameraThirdPersonZ(float value) { initOffsetCameraThirdPersonZ = value; }
	float CalculateHorizontalDistanceOfTarget();
	float CalculateVerticalDistanceOfTarget();
	float CalculateDistanceOfTarget();
	void SetOffsetThirdPersonY(float value) { offsetThirdPersonY = value; }
	void SetTargetThirdPerson(Entity* target)
	{
		targetThirdPerson = target;
		if (targetThirdPerson != NULL)
			lastPositionTarget = targetThirdPerson->transform.position;
	}
	AxisAlignedBoundingBox* GetActualFrustrumInUse() { return _actualFrustrumInUse; }
	Entity* GetTargetThirdPerson() { return targetThirdPerson; }
	void Draw(bool& wireFrameActive) override;
	void UseCamera(Shader& shader, glm::mat4 trsCamera);
	void SetViewFirstPerson();
	void SetViewThirdPerson();
	void SetProjectionPerspective(float FOV, float aspect, float, float front);
	void SetProjectionOrtho(float left, float right, float bottom, float top, float, float front);
	TypeProjectionCamera typeProjectionCamera;
	ProjectionDataPerspective projectionDataPerspective;
	ProjectionDataOrtho projectionDataOrtho;
	Camera(Renderer* _render, TypeProjectionCamera _typeProjectionCamera);
	~Camera();
	void InitCamera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch);
	glm::mat4 CalculateViewMatrix();
	void UpdateCamera();
	void SetPitch(float p);
	void SetYaw(float y);
	float GetPitch();
	float GetYaw();

	string GetClassName() override;

	void SetDataPerspective(float FOV, float sizeScreenX, float sizeScreenY, float _near, float front);

	void SetDataOrtho(float left, float right, float bottom, float top, float _near, float front);

	void ChangePerspective(TypeProjectionCamera _typeProjectionCamera);

	void SetFrustrumCulling();

	void UseProjection();

	glm::mat4 getViewMat();

	glm::mat4 getProjMat();

	virtual void SetRotationY(float rotY);
	virtual void SetRotationX(float rotX);
	void RotateCameraX(float speed);
	void RotateCameraY(float speed);
	void RotateCameraZ(float speed);

	void SetTypeCamera(TypeCamera value) { typeCamera = value; }
	TypeCamera GetTypeCamera() { return typeCamera; }
};
#endif