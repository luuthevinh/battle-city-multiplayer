#include "Wall.h"
#include "Base\SpriteManager.h"

Wall::Wall(eObjectId id) : GameObject(id)
{
}

Wall::Wall(Buffer & buffer) : GameObject(buffer)
{
}

Wall::~Wall()
{
}

Wall * Wall::createWithType(eObjectId id)
{
	Wall* wall = new Wall(id);
	if (wall && wall->init())
	{
		wall->autorelease();
		return wall;
	}

	CC_SAFE_DELETE(wall);
	return nullptr;
}

Wall * Wall::createWithBuffer(Buffer & buffer)
{
	Wall* wall = new Wall(buffer);
	if (wall && wall->init())
	{
		wall->autorelease();
		return wall;
	}

	CC_SAFE_DELETE(wall);
	return nullptr;
}

bool Wall::init()
{
	_sprite = Sprite::createWithSpriteFrameName(SpriteManager::getInstance()->getObjectName(eObjectId::BRICK_WALL) + "_05.png");
	this->addChild(_sprite);

	auto body = PhysicsBody::createBox(_sprite->getContentSize(), PhysicsMaterial(0, 0, 0));
	this->setPhysicsBody(body);

	body->getShapes().at(0)->setSensor(true);
	body->setContactTestBitmask(0x1);

	this->updateDirection();

	return true;
}

void Wall::updateWithStatus(eStatus status)
{
	switch (status)
	{
	case DIE:
	{
		this->runAction(RemoveSelf::create(true));
		break;
	}
	case NORMAL:
		break;
	case PROTECTED:
		break;
	case RUNNING:
		break;
	case STAND:
		break;
	default:
		break;
	}
}

void Wall::setDirection(eDirection direction)
{
	GameObject::setDirection(direction);

	this->updateDirection();
}

void Wall::updateDirection()
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
