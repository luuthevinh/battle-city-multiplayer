#ifndef __TANK_H__
#define __TANK_H__

#include "../Base/GameObject.h"

class Tank : public GameObject
{
public:
	Tank(eObjectId id);
	~Tank();

	virtual bool init() override;
	virtual void update(float dt) override;

	virtual void setDirection(eDirection direction);
	virtual eDirection getDirection();

	virtual void setVelocity(float velocity);
	virtual float getVeloctiy();

	virtual void onChanged() override;
private:
	eDirection _direction;
	float _velocity;

	void updatePosition(float dt);
};

#endif // !__TANK_H__
