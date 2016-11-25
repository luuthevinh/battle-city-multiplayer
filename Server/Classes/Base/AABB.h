#ifndef __SWEPT_AABB_H__
#define __SWEPT_AABB_H__

#include "GameObject.h"

class AABB
{
public:
	AABB();
	~AABB();

	float checkCollision(const GameObject &object, const GameObject &other, eDirection &result, float dt);
	bool isColliding(const Rect &object, const Rect &other);
	bool isColliding(const Rect& object, const Rect &other, float & moveX, float & moveY, float dt);
	Rect getSweptBroadphaseRect(const GameObject &object, float dt);

	eDirection getSideOverlap(const Rect &rect, const Rect &otherRect);

	void setOffset(float offset);
	void setOffset(float x, float y);
	
private:
	float _dxEntry, _dyEntry, _dxExit, _dyExit;
	float _txEntry, _tyEntry, _txExit, _tyExit;

	float _offsetX;
	float _offsetY;
};

#endif // !__SWEPT_AABB_H__
