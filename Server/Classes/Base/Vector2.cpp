#include "Vector2.h"
#include <math.h>

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(float xx, float yy)
{
	x = xx;
	y = yy;
}

Vector2::Vector2(const Vector2 & other)
{
	x = other.x;
	y = other.y;
}

Vector2::~Vector2()
{
}

Vector2 Vector2::operator+(const Vector2 & v) const
{

	return Vector2(v.x + x, v.y + y);
}

Vector2 Vector2::operator-(const Vector2 & v) const
{
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator*(const Vector2 & v) const
{
	return Vector2(x * v.x, y * v.y);
}

Vector2 Vector2::operator*(float value) const
{
	return Vector2(x * value, y * value);
}

Vector2 Vector2::operator/(const Vector2 & v) const
{
	return Vector2(x / v.x, y / v.y);
}

Vector2 Vector2::operator/(float value) const
{
	return Vector2(x / value, y / value);
}

bool Vector2::operator==(const Vector2 & v) const
{
	return x == v.x && y == v.y;
}

bool Vector2::operator!=(const Vector2 & v) const
{
	return x != v.x || y != v.y;
}

bool Vector2::operator>(const Vector2 & v) const
{
	if (x == v.x)
	{
		return y > v.y;
	}
	return x > v.x;
}

bool Vector2::operator<(const Vector2 & v) const
{
	if (x == v.x)
	{
		return y < v.y;
	}
	return x < v.x;
}

float Vector2::lenght() const
{
	return sqrt(x*x + y*y);
}

