#ifndef __RECT_H__
#define __RECT_H__

#include "Vector2.h"

class Rect
{
public:
	Rect(float x, float y, float width, float height);
	~Rect();

	Vector2 position;

	float width;
	float height;

	float getMinX() const;
	float getMinY() const;

	float getMidX() const;
	float getMidY() const;

	float getMaxX() const;
	float getMaxY() const;

private:

};

#endif // !__RECT_H__
