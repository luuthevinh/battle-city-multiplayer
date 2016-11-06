#ifndef __BULLET_H__
#define __BULLET_H__

#include "..\Base\Definitions.h"
#include "..\Base\GameObject.h"
#include "..\Base\IOnlineObject.h"

class Bullet : public GameObject, public IOnlineObject
{
public:
	Bullet();
	~Bullet();

	static Bullet* create(const Vec2& position, eDirection direction);

	virtual bool init() override;
	virtual void update(float dt) override;

	// Inherited via IOnlineObject
	virtual void updateWithPacket(const Packet & packet) override;

	virtual const Packet & getPacket() override;

	void setDirection(eDirection direction);
	eDirection getDirection();

	virtual void setStatus(eStatus status) override;

private:
	eDirection _direction;
	float _speed;


};

#endif // !__BULLET_H__
