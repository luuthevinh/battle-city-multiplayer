#include "Water.h"
#include "Base\SpriteManager.h"

Water::Water() : Wall(eObjectId::WATER_WALL)
{
}

Water::~Water()
{
}

bool Water::init()
{
	if (!Wall::init())
		return false;

	auto animateName = SpriteManager::getInstance()->getObjectName(this->getId());
	_sprite->runAction(RepeatForever::create(SpriteManager::getInstance()->getAnimate(animateName)));

	return true;
}
