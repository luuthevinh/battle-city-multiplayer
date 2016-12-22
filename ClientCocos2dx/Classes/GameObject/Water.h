#ifndef __WALTER_WALL_H___
#define __WALTER_WALL_H___

#include "Wall.h"

class Water : public Wall
{
public:
	Water();
	~Water();

	virtual bool init() override;

private:

};

#endif // !__WALTER_WALL_H___
