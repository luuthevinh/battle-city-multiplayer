#ifndef __BULLET_H__
#define __BULLET_H__

#include "..\Base\Definitions.h"
#include "..\Base\GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet();
	~Bullet();

	static Bullet* create(const Vec2& position, eDirection direction);
	static Bullet* createWithBuffer(Buffer& buffer);

	virtual bool init() override;
	virtual void update(float dt) override;

	void setDirection(eDirection direction);
	eDirection getDirection();

	virtual void updateWithStatus(eStatus status) override;
	virtual void predict(float dt) override;

	virtual Buffer* Bullet::serialize() override;
	virtual void deserialize(Buffer & data) override;

	virtual void setOwner(GameObject* owner);
	virtual GameObject* getOwner();

	bool onContactBegin(PhysicsContact& contact);
	void contactWithOtherObject(GameObject* object);

	void reconcile(Buffer &data) override;

	int getOwnerTag();

private:
	eDirection _direction;
	eBulletLevel _level;
	float _speed;

	void explode();

	GameObject* _owner;
	int _ownerTag;

	void checkCollisionWithBouding();
	float getBulletSpeed();
};

#endif // !__BULLET_H__
