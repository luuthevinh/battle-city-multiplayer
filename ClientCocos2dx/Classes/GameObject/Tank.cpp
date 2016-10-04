#include "Tank.h"

Tank::Tank(eObjectId id) : GameObject(id),
	_velocity(0),
	_direction(eDirection::TOP)
{
}

Tank::~Tank()
{
	for (auto i = _animations.begin(); i != _animations.end(); i++)
	{
		i->second->release();
	}
}

bool Tank::init()
{
	// update object
	this->scheduleUpdate();

	_sprite = Sprite::create();
	this->addChild(_sprite);

	_animations[eDirection::TOP] = SpriteManager::getInstance()->getAnimate(this->getName() + "_up");
	_animations[eDirection::BOTTOM] = SpriteManager::getInstance()->getAnimate(this->getName() + "_bottom");
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
	case TOP:
		this->setPositionY(this->getPositionY() + _velocity * dt);
		break;
	case RIGHT:
		this->setPositionX(this->getPositionX() + _velocity * dt);
		break;
	case BOTTOM:
		this->setPositionY(this->getPositionY() - _velocity * dt);
		break;
	default:
		break;
	}
}

void Tank::setDirection(eDirection direction)
{
	_direction = direction;
}

eDirection Tank::getDirection()
{
	return _direction;
}
