#ifndef __ICE_WALL_H__
#define __ICE_WALL_H__

#include "Wall.h"

class Ice : public Wall
{
public:
	Ice();
	~Ice();

	virtual bool init() override;

private:

};

#endif // !__ICE_WALL_H__
