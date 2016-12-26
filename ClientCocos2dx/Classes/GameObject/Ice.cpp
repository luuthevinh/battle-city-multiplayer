#include "Ice.h"

Ice::Ice() : Wall(eObjectId::ICE_WALL)
{
}

Ice::~Ice()
{
}

bool Ice::init()
{
	if (!Wall::init())
	{
		return false;
	}

	this->setZOrder(ICE_Z_INDEX);

	return true;
}
