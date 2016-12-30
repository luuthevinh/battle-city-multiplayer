#ifndef __WATER_H__
#define __WATER_H__

#include "Wall.h"

class Water : public Wall
{
public:
	Water();
	~Water();

	virtual bool init() override;
	virtual void update(float dt) override;

private:

};


#endif // !__WATER_H__
