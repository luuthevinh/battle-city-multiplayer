#include "Rect.h"

Rect::Rect(float x, float y, float w, float h)
{
	position.x = x;
	position.y = y;
	width = w;
	height = h;
}

Rect::~Rect()
{
}

float Rect::getMinX() const
{
	return position.x;
}

float Rect::getMinY() const
{
	return position.y;
}

float Rect::getMidX() const
{
	return position.x + width / 2;
}

float Rect::getMidY() const
{
	return position.x + height / 2;
}

float Rect::getMaxX() const
{
	return position.x + width;
}

float Rect::getMaxY() const
{
	return position.y + height;
}
