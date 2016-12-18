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

	virtual void deserialize(Buffer & data) override;
private:
	eDirection _direction;
	float _speed;


};

#endif // !__BULLET_H__
