#ifndef __GRASS_WALL_H__
#define __GRASS_WALL_H__

#include "Wall.h"

class Grass : public Wall
{
public:
	Grass();
	~Grass();

	virtual bool init() override;

private:

};

#endif // !__GRASS_WALL_H__
