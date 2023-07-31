#ifndef EMPTYOBJECT_H
#define EMPTYOBJECT_H

#include "PrivateClass/Export.h"
#include "Entity.h"

class ENGINE_API EmptyObject : public Entity
{
private:
	void BindBuffer() override;
	void SetEnableDrawAABB(bool value) override;
public:
	void Draw(bool& wireFrameActive) override;
	EmptyObject(Renderer* renderer);
	string GetNameOfClass() override;
	~EmptyObject();

};

#endif
