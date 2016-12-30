#include "Steel.h"

#include "Steel.h"
#include "..\Scene\PlayScene.h"

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

	_health = 1;

	return true;
}

void Steel::gotHit(Damage* damage)
{
	if (damage->getValue() == 1)
		return;

	_health -= damage->getValue();

	if (_health <= 0)
	{
		this->setStatus(eStatus::DIE);
		this->onChanged();

		this->updateMap();
		delete damage;

		return;
	}

	delete damage;
}

void Steel::updateMap()
{
	auto scene = (PlayScene*)_parentScene;
	if (scene == nullptr)
		return;

	scene->updateMap(this->getPosition(), eTiledObjectId::NONE_TILE);
}