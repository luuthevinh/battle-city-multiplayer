#ifndef __TANK_H__
#define __TANK_H__

#include <map>

#include "Base\GameObject.h"
#include "Base\SpriteManager.h"
#include "Base\IOnlineObject.h"

class Tank : public GameObject, public IOnlineObject
{
public:
	Tank(eObjectId id);
	~Tank();

	static Tank* create(eObjectId id);

	virtual bool init() override;
	virtual void update(float dt) override;
	virtual void updatePosition(float dt);

	virtual void setDirection(eDirection direction);
	virtual eDirection getDirection();

	// Inherited via IOnlineObject
	virtual void updateWithPacket(const Packet & packet) override;
	virtual const Packet & getPacket() override;

protected:
	float _velocity;
	eDirection _direction;

	std::map<eDirection, Animate*> _animations;

};

#endif // !__TANK_H__
