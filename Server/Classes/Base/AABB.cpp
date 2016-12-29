#include "AABB.h"
#include <limits>
#include <stdio.h>

#define _USE_MATH_DEFINES

#include <math.h>



AABB::AABB()
{
	_offsetX = 0;
	_offsetY = 0;
}

AABB::~AABB()
{
}

float AABB::checkCollision(const GameObject & object, const GameObject & other, eDirection &result, float dt)
{

	auto& myRect = object.getBoundingBox();
	auto& otherRect = other.getBoundingBox();

	// dùng broadphase để check trc
	Rect rect = this->getSweptBroadphaseRect(object, dt);
	if (!isColliding(rect, otherRect))
	{
		result = eDirection::NONE;
		return 1.0f;
	}

	Vector2 myVeloc = object.getVelocity() * dt;		// khoảng cách đi được trong mỗi frame
	Vector2 otherVeloc = other.getVelocity() * dt;
	Vector2 velocity = myVeloc;

	// nếu object khác cũng đang di chuyển
	if (otherVeloc != Vector2(0, 0))
	{
		velocity = myVeloc - otherVeloc;
	}

	// tìm khoảng cách giữa các cạnh
	// đang chạy qua bên phải
	if (velocity.x > 0.0f)
	{
		_dxEntry = otherRect.getMinX() - myRect.getMaxX();
		_dxExit = otherRect.getMaxX() - myRect.getMinX();
	}
	else
	{
		_dxEntry = otherRect.getMaxX() - myRect.getMinX();
		_dxExit = otherRect.getMinX() - myRect.getMaxX();
	}

	// đang chạy lên
	if (velocity.y > 0.0f)
	{
		_dyEntry = otherRect.getMinY() - myRect.getMaxY();
		_dyExit = otherRect.getMaxY() - myRect.getMinY();
	}
	else
	{
		_dyEntry = otherRect.getMaxY() - myRect.getMinY();
		_dyExit = otherRect.getMinY() - myRect.getMaxY();
	}

	// thời gian va chạm
	if (velocity.x == 0.0f)
	{
		_txEntry = -std::numeric_limits<float>::infinity();
		_txExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		_txEntry = _dxEntry / velocity.x;
		_txExit = _dxExit / velocity.x;
	}

	if (velocity.y == 0.0f)
	{
		_tyEntry = -std::numeric_limits<float>::infinity();
		_tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		_tyEntry = _dyEntry / velocity.y;
		_tyExit = _dyExit / velocity.y;
	}

	// thời gian để va chạm
	float entryTime = max(_txEntry, _tyEntry);
	// thời gian hết va chạm
	float exitTime = min(_txExit, _tyExit);

	if (entryTime > exitTime || _txEntry < 0.0f && _tyEntry < 0.0f || _txEntry > 1.0f || _tyEntry > 1.0f)
	{
		result = eDirection::NONE;
		return 1.0f;
	}

	if (abs(_dxEntry) <= _offsetX || abs(_dyEntry) <= _offsetY)
	{ 
		result = eDirection::NONE;
		return 1.0f;
	}

	// xét hướng va chạm
	if (_txEntry > _tyEntry)
	{
		if (_dxEntry > 0.0f)
		{
			result = eDirection::RIGHT;
			//printf("collided right.\n");
		}
		else
		{
			result = eDirection::LEFT;
			//printf("collided left.\n");
		}
	}
	else
	{
		if (_dyEntry > 0.0f)
		{
			result = eDirection::UP;
			//printf("collided up.\n");
		}
		else
		{
			result = eDirection::DOWN;
			//printf("collided down.\n");
		}
	}

	return entryTime;
}

bool AABB::isColliding(const Rect & object, const Rect & other)
{
	float left = other.getMinX() - object.getMaxX();
	float top = other.getMaxY() - object.getMinY();
	float right = other.getMaxX() - object.getMinX();
	float bottom = other.getMinY() - object.getMaxY();

	if ((abs(left) <= _offsetX && (abs(bottom) <= _offsetY || abs(top) <= _offsetY)) ||
		(abs(right) <= _offsetX && (abs(bottom) <= _offsetY || abs(top) <= _offsetY)))
	{
		return false;
	}

	return !(left > 0 || right < 0 || top < 0 || bottom > 0);
}

Rect AABB::getSweptBroadphaseRect(const GameObject & object, float dt)
{
	// vận tốc
	auto velocity = object.getVelocity() * dt;
	auto myRect = object.getBoundingBox();

	float x = velocity.x > 0 ? myRect.getMinX() : myRect.getMinX() + velocity.x;
	float y = velocity.y > 0 ? myRect.getMinY() : myRect.getMinY() + velocity.y;
	float w = myRect.width + abs(velocity.x);
	float h = myRect.height + abs(velocity.y);

	return Rect(x, y, w, h);
}

eDirection AABB::getSideOverlap(const Rect &rect, const Rect &otherRect)
{
	float left = otherRect.getMinX() - rect.getMaxX();
	float top = otherRect.getMaxY() - rect.getMinY();
	float right = otherRect.getMaxX() - rect.getMinX();
	float bottom = otherRect.getMinY() - rect.getMaxY();

	if (left > 0 || right < 0 || top < 0 || bottom > 0)
		return eDirection::NONE;

	// khoảng cách cạnh bên phải của other 
	// so với cạnh trái của object phải nhỏ hơn offset 
	// và y đang nằm trong top-down của object
	if (abs(right) <= _offsetX && (top >= _offsetY && abs(bottom) >= _offsetY))
		return eDirection::LEFT;

	if (abs(left) <= _offsetX && (top >= _offsetY && abs(bottom) >= _offsetY))
		return eDirection::RIGHT;

	if (abs(top) <= _offsetY && (right >= _offsetX && abs(left) >= _offsetX))
		return eDirection::DOWN;

	if (abs(bottom) <= _offsetY && (right >= _offsetX && abs(left) >= _offsetX))
		return eDirection::UP;

	return eDirection::NONE;
}

void AABB::setOffset(float offset)
{
	_offsetX = offset;
	_offsetY = offset;
}

void AABB::setOffset(float x, float y)
{
	_offsetX = x;
	_offsetY = y;
}

bool AABB::isColliding(const Rect& object, const Rect& other, float & moveX, float & moveY, float dt)
{
	moveX = moveY = 0.0f;

	float left = other.getMinX() - object.getMaxX();
	float top = other.getMaxY() - object.getMinY();
	float right = other.getMaxX() - object.getMinX();
	float bottom = other.getMinY() - object.getMaxY();

	//  kt coi có va chạm không
	//  có va chạm khi 
	//  left < 0 && right > 0 && top > 0 && bottom < 0
	if (left >= 0 || right <= 0 || top <= 0 || bottom >= 0)
		return false;

	// tính offset x, y để đi hết va chạm
	// lấy khoảng cách nhỏ nhất
	moveX = abs(left) < right ? left : right;
	moveY = top < abs(bottom) ? top : bottom;

	// chỉ lấy phần lấn vào nhỏ nhất
	if (abs(moveX) < abs(moveY))
		moveY = 0.0f;
	else
		moveX = 0.0f;

	return true;
}