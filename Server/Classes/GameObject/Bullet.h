#ifndef __BULLET_H__
#define __BULLET_H__

#include "..\Base\GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(const Vector2& position, eDirection direction);
	~Bullet();

	virtual void update(float dt) override;
	virtual void onChanged() override;

private:
	float _speed;

	void checkPosition();
};


#endif // !__BULLET_H__
