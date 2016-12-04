#ifndef __TANK_H__
#define __TANK_H__

#include <map>

#include "Base\GameObject.h"
#include "Base\SpriteManager.h"

class Tank : public GameObject
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

	virtual void setDirection(eDirection direction) override;

	bool onContactBegin(PhysicsContact &contact);

	void updateWithStatus(eStatus status) override;

	virtual void predict(float dt) override;

	virtual void setId(eObjectId id) override;

protected:
	float _velocity;

	std::map<eDirection, Animate*> _animations;

	void runAnimateByDirection(eDirection direction);
	void updateSpriteWithId();
};

#endif // !__TANK_H__
