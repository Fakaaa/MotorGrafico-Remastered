#include <glew.h>
#include <GLFW/glfw3.h>

#include "Entity.h"
#include "AxisAlignedBoundingBox.h"
#include "Gui/EngineGui.h"
#include "Camera.h"
#include "FrustrumCulling.h"

Entity::Entity(Renderer* _renderer)
{
	renderer = _renderer;

	internalData.localModel = glm::mat4(1.0f);
	internalData.globalModel = glm::mat4(1.0f);
	internalData.originModel = glm::mat4(1.0f);

	internalData.rotateX = glm::mat4(1.0f);
	internalData.rotateY = glm::mat4(1.0f);
	internalData.rotateZ = glm::mat4(1.0f);
	internalData.scale = glm::mat4(1.0f);
	internalData.translate = glm::mat4(1.0f);

	transform.globalScale = glm::vec3(1.0f);
	transform.forward = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	transform.backward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	transform.left = glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);
	transform.right = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	transform.up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	transform.down = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);

	SetPosition(0, 0, 0);
	SetRotationX(0);
	SetRotationY(0);
	SetRotationZ(0);
	SetScale(1, 1, 1);

	isModel = 0;
	nameEntity = "Entity sin nombre";
}

Entity::Entity(Renderer* _renderer, float _isModel)
{
	renderer = _renderer;

	internalData.localModel = glm::mat4(1.0f);
	internalData.globalModel = glm::mat4(1.0f);
	internalData.originModel = glm::mat4(1.0f);

	internalData.rotateX = glm::mat4(1.0f);
	internalData.rotateY = glm::mat4(1.0f);
	internalData.rotateZ = glm::mat4(1.0f);
	internalData.scale = glm::mat4(1.0f);
	internalData.translate = glm::mat4(1.0f);

	transform.globalScale = glm::vec3(1.0f);
	transform.forward = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	transform.backward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	transform.left = glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);
	transform.right = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	transform.up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	transform.down = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);

	SetPosition(0, 0, 0);
	SetRotationX(0);
	SetRotationY(0);
	SetRotationZ(0);
	SetScale(1, 1, 1);

	isModel = _isModel;

	nameEntity = "Entity sin nombre";

}

Entity::~Entity()
{
	if (axisAlignedBoundingBox != NULL) {
		delete axisAlignedBoundingBox;
		axisAlignedBoundingBox = NULL;
	}
}

void Entity::UpdateMatrixModel()
{
	internalData.localModel = internalData.translate * internalData.rotateX * internalData.rotateY * internalData.rotateZ * internalData.scale;
	internalData.originModel = internalData.localModel;

	if (parent != NULL)
		internalData.globalModel = parent->GetInternalData().globalModel * internalData.localModel;
	else
		internalData.globalModel = internalData.localModel;

	internalData.localModel = internalData.globalModel;

	transform.globalPosition = internalData.localModel * glm::vec4(1.0f);

	for (int i = 0; i < childrens.size(); i++)
	{
		childrens[i]->UpdateMatrixModel();
	}
}

Renderer* Entity::GetRenderer() {
	return renderer;
}

void Entity::CreateMyAxisAlignedBoundingBox()
{
	axisAlignedBoundingBox = new AxisAlignedBoundingBox(renderer);
}

InternalData Entity::GetInternalData()
{
	return internalData;
}

void Entity::SetPosition(float x, float y, float z)
{
	transform.position[0] = x;
	transform.position[1] = y;
	transform.position[2] = z;

	internalData.translate = glm::translate(glm::mat4(1.0f), transform.position);
	UpdateMatrixModel();
}

void Entity::SetPosition(glm::vec3 position)
{
	transform.position[0] = position.x;
	transform.position[1] = position.y;
	transform.position[2] = position.z;

	internalData.translate = glm::translate(glm::mat4(1.0f), transform.position);
	UpdateMatrixModel();
}

void Entity::SetScale(float x, float y, float z)
{
	transform.scale[0] = x;
	transform.scale[1] = y;
	transform.scale[2] = z;

	internalData.scale = glm::scale(glm::mat4(1.0f), transform.scale);
	UpdateMatrixModel();
}

void Entity::SetScale(glm::vec3 scale)
{
	transform.scale[0] = scale.x;
	transform.scale[1] = scale.y;
	transform.scale[2] = scale.z;

	internalData.scale = glm::scale(glm::mat4(1.0f), transform.scale);
	UpdateMatrixModel();
}

void Entity::SetName(string name)
{
	nameEntity = name;
}

string Entity::GetName()
{
	return nameEntity;
}

void Entity::SetRotationX(float x)
{

	transform.rotation[0] = x;
	glm::vec3 axis;
	axis[0] = 1.0f;
	axis[1] = 0.0f;
	axis[2] = 0.0f;

	internalData.rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(x), axis);
	UpdateMatrixModel();
}

void Entity::SetRotationY(float y)
{
	transform.rotation[1] = y;
	glm::vec3 axis;
	axis[0] = 0.0f;
	axis[1] = 1.0f;
	axis[2] = 0.0f;

	internalData.rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(y), axis);
	UpdateMatrixModel();
}

void Entity::SetRotationZ(float z)
{
	transform.rotation[2] = z;
	glm::vec3 axis;
	axis[0] = 0.0f;
	axis[1] = 0.0f;
	axis[2] = 1.0f;

	internalData.rotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(z), axis);
	UpdateMatrixModel();
}

void Entity::InitIsModelShader()
{
	if (renderer != NULL)
		_uniformIsModelLocation = glGetUniformLocation(renderer->GetCurrentShaderUse().getId(), "isModel");
}

void Entity::CheckIsModel()
{
	InitIsModelShader();
	glUniform1f(_uniformIsModelLocation, isModel);
}

void Entity::SetInmortalObject(bool value)
{
	InmortalObject = value;
}

glm::vec4 Entity::GetForward()
{
	transform.rotationQuaternion = EulerToQuat(transform.rotation);
	transform.forward = glm::vec4(QuatXVec(transform.rotationQuaternion, glm::vec3(0, 0, 1)), 0);

	return transform.forward;
}

glm::vec4 Entity::GetUp()
{
	transform.rotationQuaternion = EulerToQuat(transform.rotation);
	transform.up = glm::vec4(QuatXVec(transform.rotationQuaternion, glm::vec3(0, 1, 0)), 0);

	return transform.up;
}

glm::vec4 Entity::GetRight()
{
	transform.rotationQuaternion = EulerToQuat(transform.rotation);
	transform.right = glm::vec4(QuatXVec(transform.rotationQuaternion, glm::vec3(1, 0, 0)), 0);

	return transform.right;
}

#pragma region UI

void Entity::DisableMeAndChilds()
{
	SetIsAlive(false);
	for (Entity* child : childrens) {
		child->DisableMeAndChilds();
	}
}

void Entity::EnableMeAndChilds()
{
	SetIsAlive(true);
	for (Entity* child : childrens) {
		child->EnableMeAndChilds();
	}
}

void Entity::ShowUI()
{
	EngineGui::ShowEntityNodeInfo(this);
}

#pragma endregion

#pragma region JERARQUIA DE ENTIDADES (NODOS)

void Entity::_GetEntityNode(string nameID, vector<Entity*>& res)
{
	for (Entity* child : childrens) {
		if (child->GetName() == nameID) {
			res.push_back(child);
		}
		if (!res.empty()) {
			return;
		}
		child->_GetEntityNode(nameID, res);
	}
}

void Entity::_AddChildren(Entity* children, Entity* newParent)
{
	if (children == NULL)
	{
		cout << "el chidren es nulo" << endl;
		return;
	}
	for (int i = 0; i < newParent->childrens.size(); i++)
	{
		if (newParent->childrens[i] == children)
		{
			cout << "el chidren ya existe" << endl;
			return;
		}
	}
	children->parent = newParent;
	newParent->childrens.push_back(children);
	children->UpdateMatrixModel();
}

void Entity::AddChildren(Entity* children)
{
	if (children == NULL)
	{
		cout << "el chidren es nulo" << endl;
		return;
	}
	for (int i = 0; i < childrens.size(); i++)
	{
		if (childrens[i] == children)
		{
			cout << "el chidren ya existe" << endl;
			return;
		}
	}

	if (children->parent) {
		Entity* p = (Entity*)children->parent;
		p->RemoveChildren(children, this);
	}
	else
	{
		children->parent = this;
		childrens.push_back(children);
	}

	children->UpdateMatrixModel();
}

void Entity::RemoveChildren(Entity* children, Entity* newParent)
{
	int index = -1;

	for (int i = 0; i < childrens.size(); i++)
	{
		if (childrens[i] == children)
		{
			children->parent = newParent;
			bool push = true;

			for (int j = 0; j < newParent->childrens.size(); j++) {
				if (newParent->childrens[j] == children)
				{
					push = false;
					cout << "Entre" << endl;
				}
			}
			if (push)
			{
				newParent->childrens.push_back(children);
			}
			children->UpdateMatrixModel();
			index = i;
			i = childrens.size();
		}
	}
	if (index != -1) {
		childrens.erase(childrens.begin() + index);
	}
	else
	{
		Entity* entity = NULL;
		entity = GetEntityNode(children->GetName());
		if (entity != NULL)
		{
			Entity* p = NULL;
			p = (Entity*)entity->parent;
			if (p != NULL) {
				p->RemoveChildren(entity, newParent);
				_AddChildren(children, newParent);
			}
			entity->parent = newParent;
		}
	}
}

Entity* Entity::GetEntityNode(string name)
{
	vector<Entity*> vecNode;
	_GetEntityNode(name, vecNode);
	Entity* current = nullptr;
	if (!vecNode.empty()) {
		current = vecNode.at(0);
	}
	return current;
}

Entity* Entity::GetChild(int childIndex) const
{
	if (childIndex < 0 || childIndex > childrens.size()) {
		return NULL;
	}
	return childrens[childIndex];
}

void Entity::PrintTree()
{
	cout << nameEntity << endl;
	for (Entity* child : childrens) {
		child->PrintTree();
	}
}

glm::quat Entity::EulerToQuat(glm::vec3 euler)
{
	euler *= deg2rad;

	float cr = cos(euler.x * 0.5);
	float sr = sin(euler.x * 0.5);
	float cp = cos(euler.y * 0.5);
	float sp = sin(euler.y * 0.5);
	float cy = cos(euler.z * 0.5);
	float sy = sin(euler.z * 0.5);

	glm::quat q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

glm::vec3 Entity::QuatXVec(glm::quat quat, glm::vec3 vec)
{
	float x2 = quat.x * 2.0f;
	float y2 = quat.y * 2.0f;
	float z2 = quat.z * 2.0f;
	float xx2 = quat.x * x2;
	float yy2 = quat.y * y2;
	float zz2 = quat.z * z2;
	float xy2 = quat.x * y2;
	float xz2 = quat.x * z2;
	float yz2 = quat.y * z2;
	float wx2 = quat.w * x2;
	float wy2 = quat.w * y2;
	float wz2 = quat.w * z2;

	glm::vec3 res;
	res.x = (1.0f - (yy2 + zz2)) * vec.x + (xy2 - wz2) * vec.y + (xz2 + wy2) * vec.z;
	res.y = (xy2 + wz2) * vec.x + (1.0f - (xx2 + zz2)) * vec.y + (yz2 - wx2) * vec.z;
	res.z = (xz2 - wy2) * vec.x + (yz2 + wx2) * vec.y + (1.0f - (xx2 + yy2)) * vec.z;
	return res;
}

void Entity::UpdateTransformsData()
{
	transform.rotationQuaternion = EulerToQuat(transform.rotation);
	transform.forward = glm::vec4(QuatXVec(transform.rotationQuaternion, glm::vec3(0, 0, 1)), 0);
	transform.up = glm::vec4(QuatXVec(transform.rotationQuaternion, glm::vec3(0, 1, 0)), 0);
	transform.right = glm::vec4(QuatXVec(transform.rotationQuaternion, glm::vec3(1, 0, 0)), 0);
}

void Entity::CheckVisibleFrustrumCulling(vector<Entity*> ObjectsInFrustrum, vector<int>& indexObjectsVisibility, FrustrumCulling* frustrumCulling)
{
	int index = 0;
	bool parentAlive = true;

	if (parent != NULL)
		parentAlive = parent->GetIsAlive();

	for (int i = 0; i < ObjectsInFrustrum.size(); i++)
	{
		if (ObjectsInFrustrum[i] == this)
		{
			index = i;
			i = ObjectsInFrustrum.size();
		}
	}

	if (!parentAlive)
	{
		SetIsAlive(false);
		indexObjectsVisibility.push_back(index);
		DisableMeAndChilds();
		return;
	}

	frustrumCulling->CheckObjectInFrustrum(index, indexObjectsVisibility, this);

	for (Entity* child : childrens) {
		child->CheckVisibleFrustrumCulling(ObjectsInFrustrum, indexObjectsVisibility, frustrumCulling);
	}
}

glm::vec3* Entity::GetAABBGlobalPositions()
{
	glm::vec3 auxVec[8];

	for (int i = 0; i < 8; i++)
	{
		if (GetNameOfClass() == "Primitive3D")
		{
			auxVec[i] = axisAlignedBoundingBox->GetAABBPositions()[i] + transform.globalPosition - transform.scale;
		}
		else
		{
			auxVec[i] = axisAlignedBoundingBox->GetAABBPositions()[i] + transform.globalPosition - transform.globalScale;
		}
	}

	return auxVec;
}
#pragma endregion 
