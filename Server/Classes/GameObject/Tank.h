#ifndef __TANK_H__
#define __TANK_H__

#include <vector>
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
	
	virtual void setDirection(eDirection direction) override;

protected:
	float _velocity;

	std::vector<eDirection> _inputTurns;

	void updatePosition(float dt);
	void updatePosition(float dt, float velocity);

	eDirection _collidingSide;

	void updateBoundingBoxPosition();
	void turnWithInputQueue(float dt);
	void updateWithInputQueue(float dt);
	void generateInput(eDirection direction, int number);

	void move(float distance);
	void checkCollidingSide(const Rect& otherRect);
	bool isCollidingAtSide(eDirection side);

	eDirection getDirectionInQueue();
	void updateDirection(eDirection direction);
};

#endif // !__TANK_H__
