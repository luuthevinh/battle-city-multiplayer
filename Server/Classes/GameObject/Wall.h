#ifndef __WALL_H__
#define __WALL_H__

#include "..\Base\GameObject.h"

class Wall : public GameObject
{
public:
	Wall(eObjectId id);
	~Wall();

	static Wall* createWithPosition(eObjectId type, const Vector2 &position);

	virtual bool init() override;

	virtual void checkCollision(GameObject& object, float dt) override;

	virtual void updateBoundingBox();

private:
	
};

#endif // !__WALL_H__
