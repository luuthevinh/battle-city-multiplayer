#include "Wall.h"
#include "Base\SpriteManager.h"
#include "Brick.h"
#include "Steel.h"
#include "Water.h"
#include "Grass.h"
#include "Ice.h"

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
	Wall* wall = Wall::getNewWallById(id);

	if (wall && wall->init())
	{
		wall->autorelease();
		return wall;
	}

	CC_SAFE_DELETE(wall);
	return nullptr;
}

Wall * Wall::createInfo(Buffer & buffer)
{
	buffer.setBeginRead(0);
	eDataType type = (eDataType)buffer.readInt();
	if (type != eDataType::OBJECT)
		return nullptr;

	eObjectId objectId = (eObjectId)buffer.readInt();

	Wall* wall = Wall::getNewWallById(objectId);

	if (wall)
	{
		wall->initWithBuffer(buffer);
		return wall;
	}

	CC_SAFE_DELETE(wall);
	return nullptr;
}

Wall * Wall::createGameObject(GameObject * info)
{
	Wall* wall = Wall::getNewWallById(info->getId());

	if (wall && wall->init())
	{
		wall->deserialize(*info->serialize());
		wall->autorelease();
		return wall;
	}

	CC_SAFE_DELETE(wall);
	return nullptr;
}

bool Wall::init()
{
	_sprite = Sprite::createWithSpriteFrameName(SpriteManager::getInstance()->getObjectName(this->getId()) + "_00.png");
	this->addChild(_sprite);

	auto body = PhysicsBody::createBox(_sprite->getContentSize(), PhysicsMaterial(0, 0, 0));
	this->setPhysicsBody(body);

	body->getShapes().at(0)->setSensor(true);
	body->setContactTestBitmask(eObjectId::BULLET | eObjectId::WHITE_TANK | eObjectId::YELLOW_TANK | eObjectId::GREEN_TANK);

	_direction = eDirection::NONE;

	this->setContentSize(Size(16.0f, 16.0f));

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

Wall * Wall::getNewWallById(eObjectId id)
{
	switch (id)
	{
	case BRICK_WALL:
		return new Brick();
		break;
	case STEEL_WALL:
		return new Steel();
		break;
	case GRASS_WALL:
		return new Grass();
		break;
	case ICE_WALL:
		return new Ice();
		break;
	case WATER_WALL:
		return new Water();
	default:
		break;
	}

	return nullptr;
}
