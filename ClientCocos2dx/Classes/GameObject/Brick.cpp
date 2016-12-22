#include "Brick.h"
#include "Base\SpriteManager.h"

Brick::Brick() : Wall(eObjectId::BRICK_WALL)
{
}

Brick::~Brick()
{
}

bool Brick::init()
{
	if (!Wall::init())
	{
		return false;
	}


	return true;
}

void Brick::setDirection(eDirection direction)
{
	GameObject::setDirection(direction);
	this->updateDirection();
}

void Brick::updateDirection()
{
	switch (_direction)
	{
	case LEFT:
		_sprite->setSpriteFrame(SpriteManager::getInstance()->getObjectName(eObjectId::BRICK_WALL) + "_08.png");
		break;
	case UP:
		_sprite->setSpriteFrame(SpriteManager::getInstance()->getObjectName(eObjectId::BRICK_WALL) + "_09.png");
		break;
	case RIGHT:
		_sprite->setSpriteFrame(SpriteManager::getInstance()->getObjectName(eObjectId::BRICK_WALL) + "_06.png");
		break;
	case DOWN:
		_sprite->setSpriteFrame(SpriteManager::getInstance()->getObjectName(eObjectId::BRICK_WALL) + "_07.png");
		break;
	default:
		break;
	}
}
