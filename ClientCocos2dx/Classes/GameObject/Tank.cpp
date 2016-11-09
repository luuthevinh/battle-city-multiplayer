#include "Tank.h"

// shared
#include "..\Server\Classes\Shared\Buffer.h"

Tank::Tank(eObjectId id) : GameObject(id),
	_velocity(0),
	_direction(eDirection::UP)
{
	_buffer = new Buffer(25);
}

Tank::Tank(Buffer& data)
{
	_buffer = new Buffer(25);
	this->deserialize(data);
}

Tank::~Tank()
{
	for (auto i = _animations.begin(); i != _animations.end(); i++)
	{
		i->second->release();
	}

	delete _buffer;
}

Tank* Tank::create(eObjectId id)
{
	Tank* tank = new(std::nothrow) Tank(id);
	if (tank && tank->init())
	{
		tank->autorelease();
		return tank;
	}
	else
	{
		delete tank;
		tank = nullptr;
		return nullptr;
	}
}

Tank * Tank::createWithBuffer(Buffer &data)
{
	Tank* tank = new(std::nothrow) Tank(eObjectId::YELLOW_TANK);
	if (tank && tank->init())
	{
		tank->deserialize(data);
		tank->autorelease();
		return tank;
	}
	else
	{
		delete tank;
		tank = nullptr;
		return nullptr;
	}
}

bool Tank::init()
{
	// update object
	this->scheduleUpdate();

	_sprite = Sprite::create();
	this->addChild(_sprite);

	_animations[eDirection::UP] = SpriteManager::getInstance()->getAnimate(this->getName() + "_up");
	_animations[eDirection::DOWN] = SpriteManager::getInstance()->getAnimate(this->getName() + "_down");
	_animations[eDirection::LEFT] = SpriteManager::getInstance()->getAnimate(this->getName() + "_left");
	_animations[eDirection::RIGHT] = SpriteManager::getInstance()->getAnimate(this->getName() + "_right");

	for (auto i = _animations.begin(); i != _animations.end(); i++)
	{
		i->second->retain();
	}

	_sprite->runAction(_animations[_direction]);

	return true;
}

void Tank::update(float dt)
{
	// update postion
	this->updatePosition(dt);
}

void Tank::updatePosition(float dt)
{
	if (_velocity == 0)
		return;

	switch (_direction)
	{
	case LEFT:
		this->setPositionX(this->getPositionX() - _velocity * dt);
		break;
	case UP:
		this->setPositionY(this->getPositionY() + _velocity * dt);
		break;
	case RIGHT:
		this->setPositionX(this->getPositionX() + _velocity * dt);
		break;
	case DOWN:
		this->setPositionY(this->getPositionY() - _velocity * dt);
		break;
	default:
		break;
	}
}

void Tank::setDirection(eDirection direction)
{
	if (_direction == direction || direction <= 0 || direction > 4)
		return;

	_direction = direction;

	_sprite->stopAllActions();

	if ((_status & eStatus::RUNNING) == eStatus::RUNNING)
	{
		_sprite->runAction(RepeatForever::create(_animations[_direction]));
	}
	else
	{
		_sprite->runAction(_animations[_direction]);
	}
}

eDirection Tank::getDirection()
{
	return _direction;
}

Buffer* Tank::serialize()
{
	_buffer->setIndex(0);
	_buffer->setBeginRead(0);
	
	_buffer->writeInt(eDataType::OBJECT);
	_buffer->writeInt(this->getId());
	_buffer->writeInt(this->getTag());
	_buffer->writeInt(this->getStatus());
	_buffer->writeByte(this->getDirection());
	_buffer->writeFloat(this->getPosition().x);
	_buffer->writeFloat(this->getPosition().y);

	return _buffer;
}

void Tank::deserialize(Buffer & data)
{
	data.setBeginRead(0);

	eDataType type = (eDataType)data.readInt();
	if (type != eDataType::OBJECT)
		return;

	this->setType(type);
	this->setId((eObjectId)data.readInt());
	this->setTag(data.readInt());
	this->setStatus((eStatus)data.readInt());
	this->setDirection((eDirection)data.readByte());
	this->setPosition(data.readFloat(), data.readFloat());

	data.setBeginRead(0);
}

