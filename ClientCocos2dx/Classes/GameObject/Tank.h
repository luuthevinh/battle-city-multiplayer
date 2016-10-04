#ifndef __TANK_H__
#define __TANK_H__

#include <map>

#include "Base\GameObject.h"
#include "Base\SpriteManager.h"

class Tank : public GameObject
{
public:
	Tank(eObjectId id);
	~Tank();

	virtual bool init() override;
	virtual void update(float dt) override;
	virtual void updatePosition(float dt);

	virtual void setDirection(eDirection direction);
	virtual eDirection getDirection();


protected:
	float _velocity;
	eDirection _direction;

	std::map<eDirection, Animate*> _animations;
};

#endif // !__TANK_H__
