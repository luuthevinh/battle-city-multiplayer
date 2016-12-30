#include "Water.h"

Water::Water() : Wall(eObjectId::WATER_WALL)
{
}

Water::~Water()
{
}

bool Water::init()
{
	if (!Wall::init())
	{
		return false;
	}

	return true;
}

void Water::update(float dt)
{

}