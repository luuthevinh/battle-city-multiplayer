#include "Steel.h"

Steel::Steel() : Wall(eObjectId::STEEL_WALL)
{
}

Steel::~Steel()
{
}

bool Steel::init()
{
	if (!Wall::init())
	{
		return false;
	}

	return true;
}
