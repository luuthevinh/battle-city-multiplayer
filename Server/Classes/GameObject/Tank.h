#ifndef __TANK_H__
#define __TANK_H__

#include "..\Base\GameObject.h"
#include "..\Shared\Serializable.h"

class Tank : public GameObject, public Serializable
{
public:
	Tank(eObjectId id);
	Tank(Buffer& data);
	~Tank();

	virtual bool init() override;
	virtual void update(float dt) override;

	virtual void setVelocity(float velocity);
	virtual float getVeloctiy();

	virtual void onChanged() override;

	// Inherited via ISerializable
	virtual Buffer* serialize() override;
	virtual void deserialize(Buffer& data) override;

private:
	float _velocity;

	void updatePosition(float dt);

};

#endif // !__TANK_H__
