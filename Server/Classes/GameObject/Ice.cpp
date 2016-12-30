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

	return true;
}

void Ice::update(float dt)
{

}