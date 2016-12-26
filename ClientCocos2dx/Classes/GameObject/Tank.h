#ifndef __TANK_H__
#define __TANK_H__

#include <map>

#include "Base\GameObject.h"
#include "Base\SpriteManager.h"

#include "..\Server\Classes\Shared\DataPacket.h"

class Tank : public GameObject
{
public:
	Tank(eObjectId id);
	Tank(Buffer& data);
	~Tank();

	static Tank* create(eObjectId id);
	static Tank* createWithBuffer(Buffer& data);

	virtual void createBuffer() override;
	virtual bool init() override;
	virtual void update(float dt) override;
	virtual void updatePosition(float dt);

	virtual void setDirection(eDirection direction) override;

	bool onContactBegin(PhysicsContact &contact);
	void onContactSeparate(PhysicsContact &contact);
	bool onPreSolve(PhysicsContact &contact, PhysicsContactPreSolve& presolve);

	void updateWithStatus(eStatus status) override;

	virtual void setId(eObjectId id) override;

	virtual void shoot();
	virtual void move(eDirection direction, float dt);

	virtual Buffer* serialize() override;
	virtual void deserialize(Buffer & data) override;

	virtual void setTankLevel(eTankLevel level);
	virtual eTankLevel getTankLevel();

	virtual unsigned int getBufferSize() override;

protected:
	float _velocity;
	int _bulletCounter;
	eTankLevel _tankLevel;
	eDirection _oldDirection;

	std::map<eDirection, Animate*> _animations;
	std::queue<CommandPacket*> _commandQueue;
	std::map<GameObject*, eDirection> _collidingObjects;

	virtual void updateWithCommand(CommandPacket* commad, float dt);

	void runAnimateByDirection(eDirection direction);
	void updateSprite();
	int getMaxBullet();

	void fixPositionForTurn();
	bool isCollidingAtSide(eDirection side);
	void removeCollidingObject(GameObject* object);
	void fixPosition(eDirection direction, GameObject* other);
};

#endif // !__TANK_H__
