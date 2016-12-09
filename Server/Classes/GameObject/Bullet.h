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

	virtual void setOwner(GameObject* owner);
	virtual GameObject* getOwner();

	virtual void setDamageValue(int value);
	virtual int getDamageValue();

private:
	float _speed;

	void checkPosition();
	void updateBoudingBox();
	void explode();

	AABB* _collisionChecker;

	GameObject* _owner;
	int _damageValue;

	int _damagedObjectCounter;
};


#endif // !__BULLET_H__
