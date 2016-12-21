#ifndef __TANK_H__
#define __TANK_H__

#include <vector>
#include <map>
#include <queue>
#include "..\Base\GameObject.h"

#include "..\Shared\DataPacket.h"

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
	
	virtual void setDirection(eDirection direction) override;
	
	virtual void setNumberOfBullets(int number);
	virtual int getNumberOfBullets();

	virtual void handleData(Serializable* data) override;

	virtual void move(eDirection direction, float dt);
	virtual void shoot();

	virtual void onContactBegin(GameObject& object);

protected:
	float _velocity;
	float _remainMoveForTurn;

	int _bulletCounter;

	eTankLevel _tankLevel;

	std::vector<eDirection> _inputTurns;

	eDirection _collidingSide;
	std::map<eDirection, int> _objectCollidingCounter;

	void updatePosition(float dt);
	void updatePosition(float dt, float velocity);
	void updateBoundingBoxPosition();
	void updateWithInputQueue(float dt);

	void turnWithInputQueue(float dt);

	void generateInput(eDirection direction, int number);

	void move(float distance);

	void checkCollidingSide(GameObject& other);
	bool isCollidingAtSide(eDirection side);

	eDirection getDirectionInQueue();

	void updateDirection(eDirection direction);

	int getMaxBullet();

	void updateWithCommand(CommandPacket* commad, float dt);

	std::queue<CommandPacket*> _commandQueue;
};

#endif // !__TANK_H__
