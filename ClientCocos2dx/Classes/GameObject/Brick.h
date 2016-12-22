#ifndef __BRICK_WALL_H__
#define __BRICK_WALL_H__

#include "Wall.h"

class Brick : public Wall
{
public:
	Brick();
	~Brick();

	virtual bool init() override;
	virtual void setDirection(eDirection direction) override;

private:
	void updateDirection();
};


#endif // !__BRICK_WALL_H__
