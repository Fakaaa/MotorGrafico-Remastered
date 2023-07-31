#include "EmptyObject.h"
#include <glew.h>
#include <GLFW/glfw3.h>
#include "AxisAlignedBoundingBox.h"

EmptyObject::EmptyObject(Renderer* render) :Entity(render)
{
	InmortalObject = true;
}

void EmptyObject::BindBuffer() {}
void EmptyObject::SetEnableDrawAABB(bool value)
{
	if (axisAlignedBoundingBox != NULL)
		axisAlignedBoundingBox->SetEnableDraw(value);
}
void EmptyObject::Draw(bool& wireFrameActive)
{
	renderer->Draw(0, renderer->GetCurrentShaderUse(), internalData.localModel, wireFrameActive);
}

string EmptyObject::GetNameOfClass()
{
	return "EmptyObject";
}

EmptyObject::~EmptyObject() {}
