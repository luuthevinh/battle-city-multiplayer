#ifndef __STEEL_H__
#define __STEEL_H__

#include "Wall.h"

class Steel : public Wall
{
public:
	Steel();
	~Steel();

	virtual bool init() override;

	virtual void gotHit(Damage* damage) override;

private:
	int _health;
	void updateMap();
};


#endif // !__STEEL_H__
