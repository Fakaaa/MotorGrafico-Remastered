#include "Entity.h"
#include <glew.h>
#include <GLFW/glfw3.h>
#include "AxisAlignedBoundingBox.h"
#include "Gui/EngineGui.h"
#include "Camera.h"

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

	transform.forward = glm::vec3(0.0f, 0.0f, 1.0f);
	transform.backward = glm::vec3(0.0f, 0.0f, -1.0f);
	transform.left = glm::vec3(-1.0f, 0.0f, 0.0f);
	transform.right = glm::vec3(1.0f, 0.0f, 0.0f);
	transform.up = glm::vec3(0.0f, 1.0f, 0.0f);
	transform.down = glm::vec3(0.0f, -1.0f, 0.0f);

	SetPosition(0, 0, 0);
	SetRotationX(0);
	SetRotationY(0);
	SetRotationZ(0);
	SetScale(1, 1, 1);

	isModel = 0;
	amountIndicesTrue = 0;
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

	transform.forward = glm::vec3(0.0f, 0.0f, 1.0f);
	transform.backward = glm::vec3(0.0f, 0.0f, -1.0f);
	transform.left = glm::vec3(-1.0f, 0.0f, 0.0f);
	transform.right = glm::vec3(1.0f, 0.0f, 0.0f);
	transform.up = glm::vec3(0.0f, 1.0f, 0.0f);
	transform.down = glm::vec3(0.0f, -1.0f, 0.0f);

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
	//cout << _uniformIsModelLocation << endl;
	glUniform1f(_uniformIsModelLocation, isModel);
}

void Entity::SetInmortalObject(bool value)
{
	InmortalObject = value;
}

glm::vec3 Entity::GetForward()
{
	float dx = (float)(1 * mathLibrary.sin(mathLibrary.ToRadians(transform.rotation.y)));
	float dz = (float)(1 * mathLibrary.cos(mathLibrary.ToRadians(transform.rotation.y)));

	return glm::vec3(dx, 0, dz);
}


#pragma region UI

void Entity::SetIndexBSPPlanes(int plane1, int plane2, int plane3)
{
	indexBSP[0] = plane1;
	indexBSP[1] = plane2;
	indexBSP[2] = plane3;
}

bool Entity::CheckAmountIndicesTrue(Camera* camera)
{
	for (int i = 0; i < amountPlanesBSP; i++)
	{
		if (camera->GetIndicesBSP()[i] == this->GetIndicesBSP()[i])
		{
			if (amountIndicesTrue < amountPlanesBSP)
				amountIndicesTrue++;
		}
		else
		{
			if (amountIndicesTrue > 0)
				amountIndicesTrue--;
		}

		if (amountIndicesTrue == amountPlanesBSP)
		{
			amountIndicesTrue = 0;
			return true;
		}
		else
		{
			amountIndicesTrue = 0;
			return false;
		}
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
			//cout << "ENTRE CHE" << endl;
			children->parent = newParent;
			bool push = true;

			for (int j = 0; j < newParent->childrens.size(); j++) {
				//cout << children->GetName() << " = " << newParent->childrens[j]->GetName() << endl;
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
			//cout << "Parent: " << entity->parent->GetName() << endl;
		}
	}
	//cout << endl;
	//newParent->PrintTree();
	//cout << endl;
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
#pragma endregion 
