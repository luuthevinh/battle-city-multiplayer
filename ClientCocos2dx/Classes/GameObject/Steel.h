#ifndef __STEEL_WALL_H___
#define __STEEL_WALL_H___

#include "Wall.h"

class Steel : public Wall
{
public:
	Steel();
	~Steel();

	virtual bool init() override;

private:

};

#endif // !__STEEL_WALL_H___
