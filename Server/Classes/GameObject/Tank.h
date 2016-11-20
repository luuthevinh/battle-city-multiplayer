#ifndef __TANK_H__
#define __TANK_H__

#include "..\Base\GameObject.h"

class Tank : public GameObject
{
public:
	Tank(eObjectId id);
	Tank(Buffer& data);
	~Tank();

	virtual bool init() override;
	virtual void update(float dt) override;

	virtual void setVelocity(float velocity);
	virtual float getVeloctiy();

	virtual Vector2 getVelocity() const override;

	virtual void onChanged() override;
	virtual void checkCollision(GameObject& other, float dt) override;

private:
	float _velocity;

	void updatePosition(float dt);
	void updatePosition(float dt, float velocity);
};

#endif // !__TANK_H__
