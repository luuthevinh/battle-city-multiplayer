#ifndef __PLAYER_SERVER_H__
#define __PLAYER_SERVER_H__

#include "Definitions.h"

class Player
{
public:
	Player();
	~Player();

	void update(float dt);
	void updatePosition(float dt);

	void setPosition(float x, float y);

	void setDirection(eDirection dir);
	eDirection getDirection();

	void setVelocity(float velocity);

	float getPositionX();
	float getPositionY();

	float getVelocity();
private:
	eDirection _direction;
	float _velocity;

	float _x;
	float _y;
};

#endif // !__PLAYER_SERVER_H__
