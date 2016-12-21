#ifndef __VECTOR2_H__
#define __VECTOR2_H__

class Vector2
{
public:
	Vector2();
	Vector2(float xx, float yy);
	Vector2(const Vector2& other);
	~Vector2();

	Vector2 operator+(const Vector2& v) const;

	Vector2 operator-(const Vector2& v) const;

	Vector2 operator*(const Vector2& v) const;

	Vector2 operator*(float value) const;

	Vector2 operator/(const Vector2& v) const;
	Vector2 operator/(float value) const;

	bool operator==(const Vector2& v) const;

	bool operator!=(const Vector2& v) const;

	bool operator>(const Vector2& v) const;

	bool operator<(const Vector2& v) const;

	float lenght() const;

	float x;
	float y;

private:
};


#endif // !__VECTOR2_H__
