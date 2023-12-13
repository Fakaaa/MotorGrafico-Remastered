/*
Entity:

struct Trasform:
vector4 posicion
vector4 rotacion
vector4 escala

protected:
Renderer* renderer.
public:
Trasform trasform;
Renderer* GetRenderer();

-Metodo para mover el objeto (calcule su posicion) - retorna el resultado
-Metodo para rotar el objeto (calcule su rotacion) - retorna el resultado
-Metodo para escalar el objeto (calcule su escala) - retorna el resultado

-Funcion que calcula la trasformacion del objeto (trasform) - multiplica posicion * rotacion * escala.
*/

#ifndef ENTITY_H
#define ENTITY_H

#include "../Lib/glm/vec4.hpp"
#include "../Lib/glm/vec3.hpp"
#include "../Lib/glm/vec2.hpp"
#include "../Lib/glm/mat4x4.hpp"
#include "../Lib/glm/ext/matrix_transform.hpp"
#include "../Lib/glm/ext/matrix_clip_space.hpp"
#include "../Lib/glm/ext/scalar_constants.hpp"
#include "PrivateClass/Export.h"

#include "MathLibrary.h"

#include "Renderer.h"

class AxisAlignedBoundingBox;
class FrustrumCulling;

#include "stb_image.h"

#define ENTITY_2DIMENTIONS 1
#define ENTITY_3DIMENTIONS 2

struct ENGINE_API Transform
{
	glm::vec4 forward;
	glm::vec4 backward;
	glm::vec4 left;
	glm::vec4 right;
	glm::vec4 up;
	glm::vec4 down;

	glm::vec3 globalScale;
	glm::vec3 globalPosition;
	glm::vec3 globalRotation;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::quat rotationQuaternion;
};
struct ENGINE_API InternalData
{
	glm::mat4 localModel;
	glm::mat4 globalModel;
	glm::mat4 originModel;
	glm::mat4 translate;
	glm::mat4 rotateX;
	glm::mat4 rotateY;
	glm::mat4 rotateZ;
	glm::mat4 scale;
};

class ENGINE_API Entity
{
private:
	//JERARQUIA//
	void _GetEntityNode(string nameID, vector<Entity*>& res);
	void _AddChildren(Entity* children, Entity* newParent);
	//---------//
protected:

	void CreateMyAxisAlignedBoundingBox();

	//FRUSTRUM CULLING
	AxisAlignedBoundingBox* axisAlignedBoundingBox = NULL;
	//---------------//

	//JERARQUIA//
	Entity* parent = NULL;
	vector<Entity*> childrens;
	//-----------//

	float deg2rad = (glm::pi<float>() * 2.0f) / 360.0f;

	Renderer* renderer;
	InternalData internalData;
	void UpdateMatrixModel();
	MathLibrary mathLibrary;
	float isModel;

	unsigned int _uniformIsModelLocation;
	string nameEntity;

	//UI//
	bool isUILocked = false;
	bool showInDebug = false;
	//---//

	unsigned int _ibo;
	unsigned int _vbo;
	unsigned int _vao;

	unsigned int _uniformModelLocation;
	unsigned int _uniformViewLocation;
	unsigned int _uniformProjectionLocation;
	unsigned int _positionLocation;
	unsigned int _texLocation;
	unsigned int _colorLocation;
	unsigned int _normalLocation;

	virtual void BindBuffer() = 0;

	bool isAlive = true;
	bool InmortalObject = false;

public:
	virtual void Draw(bool& wireFrameActive) = 0;
	virtual void SetEnableDrawAABB(bool value) = 0;

	Entity(Renderer* _renderer);
	Entity(Renderer* _renderer, float _isModel);
	~Entity();
	Transform transform;
	Renderer* GetRenderer();

	InternalData GetInternalData();
	AxisAlignedBoundingBox* GetAABB() { return axisAlignedBoundingBox; }
	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(glm::vec3 position);
	virtual void SetScale(float x, float y, float z);
	virtual void SetScale(glm::vec3 scale);
	virtual void SetRotationX(float x);
	virtual void SetRotationY(float y);
	virtual void SetRotationZ(float z);
	virtual void CalculateScaleToParent();
	void SetName(string name);
	string GetName();
	void InitIsModelShader();
	void CheckIsModel();
	bool GetIsAlive() { return isAlive; }
	virtual void SetIsAlive(bool value)
	{
		isAlive = value;
	}
	void SetInmortalObject(bool value);
	bool GetIsInmortal() { return InmortalObject; }

	virtual string GetNameOfClass()
	{
		return "Entity"; // Aplicar override en todas las clases hijo.
	}
	void DisableMeAndChilds();
	void EnableMeAndChilds();

	//UI//
	void ShowUI();
	void SetShowInDebug(float value) { showInDebug = value; }
	bool GetShowInDebug() { return showInDebug; }
	void SetIsUILocked(float value) { isUILocked = value; }
	bool GetIsUILocked() { return isUILocked; }
	//------//

	// JERARQUIA // 
	Entity* GetParent() { return parent; }
	void SetParent(Entity* newParent);
	void AddChildren(Entity* children);
	void RemoveChildren(Entity* children, Entity* newParent);
	Entity* GetEntityNode(string name);
	Entity* GetChild(int childIndex) const;
	void PrintTree();
	vector<Entity*> GetChildrens() { return childrens; }
	//-----------//

	glm::quat EulerToQuat(glm::vec3 euler);
	glm::vec3 QuatXVec(glm::quat quat, glm::vec3 vec);
	void UpdateTransformsData();

	void CheckVisibleFrustrumCulling(vector<Entity*> ObjectsInFrustrum, vector<int>& indexObjectsVisibility, FrustrumCulling* FrustrumCulling);

	virtual glm::vec3* GetAABBGlobalPositions();

	glm::vec4 GetForward();
	glm::vec4 GetUp();
	glm::vec4 GetRight();
	glm::vec3 GetRootRotation();
};
#endif