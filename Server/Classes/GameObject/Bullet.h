#ifndef __BULLET_H__
#define __BULLET_H__

#include "..\Base\GameObject.h"
#include "..\Base\AABB.h"

class Bullet : public GameObject
{
public:
	Bullet(const Vector2& position, eDirection direction);
	Bullet(Buffer& buffer);
	~Bullet();

	virtual bool init() override;
	virtual void update(float dt) override;
	virtual void onChanged() override;

	virtual void checkCollision(GameObject& object, float dt) override;

	virtual Vector2 getVelocity() const override;

private:
	float _speed;

	void checkPosition();

	AABB* _collisionChecker;
};


#endif // !__BULLET_H__
