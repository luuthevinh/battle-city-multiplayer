#ifndef __TANK_H__
#define __TANK_H__

#include <map>

#include "Base\GameObject.h"
#include "Base\SpriteManager.h"

// shared
#include "..\Server\Classes\Shared\Serializable.h"

class Tank : public GameObject, public Serializable
{
public:
	Tank(eObjectId id);
	Tank(Buffer& data);
	~Tank();

	static Tank* create(eObjectId id);
	static Tank* createWithBuffer(Buffer& data);

	virtual bool init() override;
	virtual void update(float dt) override;
	virtual void updatePosition(float dt);

	virtual void setDirection(eDirection direction);
	virtual eDirection getDirection();

	// Inherited via ISerializable
	virtual Buffer* serialize() override;
	virtual void deserialize(Buffer & data) override;

protected:
	float _velocity;
	eDirection _direction;

	std::map<eDirection, Animate*> _animations;

};

#endif // !__TANK_H__
