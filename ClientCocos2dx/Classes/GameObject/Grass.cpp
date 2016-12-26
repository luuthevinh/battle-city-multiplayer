#include "Grass.h"

Grass::Grass() : Wall(eObjectId::GRASS_WALL)
{
}

Grass::~Grass()
{
}

bool Grass::init()
{
	if (!Wall::init())
	{
		return false;
	}

	this->setZOrder(GRASS_Z_INDEX);

	return true;
}
