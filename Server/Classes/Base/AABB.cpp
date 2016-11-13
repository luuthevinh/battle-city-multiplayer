#include "AABB.h"
#include <limits>
#include <stdio.h>

AABB::AABB()
{
}

AABB::~AABB()
{
}

float AABB::checkCollision(const GameObject & object, const GameObject & other, eDirection &result, float dt)
{

	Rect myRect = object.getBoundingBox();
	Rect otherRect = other.getBoundingBox();

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
		velocity = otherVeloc - myVeloc;
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

	// xét hướng va chạm
	if (_txEntry > _tyEntry)
	{
		if (_dxEntry < 0.0f)
		{
			result = eDirection::RIGHT;
			printf("collided right.\n");
		}
		else
		{
			result = eDirection::LEFT;
			printf("collided left.\n");
		}
	}
	else
	{
		if (_dyEntry < 0.0f)
		{
			result = eDirection::UP;
			printf("collided up.\n");
		}
		else
		{
			result = eDirection::DOWN;
			printf("collided down.\n");
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
	if (left > 0 || right < 0 || top < 0 || bottom > 0)
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