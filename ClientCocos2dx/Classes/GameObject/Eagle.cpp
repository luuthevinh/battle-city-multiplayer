#include "Eagle.h"
#include "Base\SpriteManager.h"

Eagle::Eagle() : GameObject(eObjectId::EAGLE)
{
}

Eagle::~Eagle()
{
}

Eagle * Eagle::createWithBufer(Buffer & buffer)
{
	buffer.setBeginRead(0);
	eDataType type = (eDataType)buffer.readInt();
	if (type != eDataType::OBJECT)
		return nullptr;

	Eagle* ret = new(std::nothrow) Eagle();

	if (ret && ret->init())
	{
		ret->initWithBuffer(buffer);
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool Eagle::init()
{
	auto name = SpriteManager::getInstance()->getObjectName(this->getId());
	_sprite = Sprite::createWithSpriteFrameName("flag_00.png");
	this->addChild(_sprite);

	auto body = PhysicsBody::createBox(_sprite->getContentSize(), PhysicsMaterial(0, 0, 0));
	this->setPhysicsBody(body);

	body->getShapes().at(0)->setSensor(true);
	body->setContactTestBitmask(eObjectId::BULLET);

	_direction = eDirection::NONE;

	this->setContentSize(Size(32.0f, 32.0f));

	return true;
}

void Eagle::updateWithStatus(eStatus status)
{
	switch (status)
	{
	case DIE:
	{
		_sprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("flag_01.png"));
		break;
	}
	default:
		break;
	}
}