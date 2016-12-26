#include "Grass.h"
#include "Base\SpriteManager.h"

Grass::Grass() : Wall(eObjectId::GRASS_WALL)
{
}

Grass::~Grass()
{
}

bool Grass::init()
{
	_sprite = Sprite::createWithSpriteFrameName(SpriteManager::getInstance()->getObjectName(this->getId()) + "_00.png");
	this->addChild(_sprite);

	_direction = eDirection::NONE;

	this->setContentSize(Size(16.0f, 16.0f));

	this->setZOrder(GRASS_Z_INDEX);

	return true;
}
