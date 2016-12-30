#ifndef __BRICK_H__
#define __BRICK_H__

#include "Wall.h"

class Brick : public Wall
{
public:
	Brick();
	~Brick();

	virtual bool init() override;

	virtual void gotHit(Damage* damage) override;
	virtual void updateBoundingBox() override;

private:
	int _health;
	void updateMap();
};


#endif // !__BRICK_H__
