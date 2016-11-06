#ifndef __VECTOR2_H__
#define __VECTOR2_H__

class Vector2
{
public:
	Vector2();
	Vector2(float xx, float yy);
	~Vector2();

	Vector2 operator+(const Vector2& v);

	Vector2 operator-(const Vector2& v);

	Vector2 operator*(const Vector2& v);

	Vector2 operator/(const Vector2& v);

	bool operator==(const Vector2& v);

	bool operator!=(const Vector2& v);

	bool operator>(const Vector2& v);

	bool operator<(const Vector2& v);

	float x;
	float y;

private:
};


#endif // !__VECTOR2_H__
