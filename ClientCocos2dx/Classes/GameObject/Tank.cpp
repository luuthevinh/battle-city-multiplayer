#include "Tank.h"
#include "Explosion.h"

// shared
#include "..\Server\Classes\Shared\Buffer.h"

Tank::Tank(eObjectId id) : GameObject(id),
	_velocity(0)
{
}

Tank::Tank(Buffer& data) : GameObject(data),
	_velocity(0)
{
}

Tank::~Tank()
{
	for (auto i = _animations.begin(); i != _animations.end(); i++)
	{
		i->second->release();
	}
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
	Tank* tank = new(std::nothrow) Tank(data);
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

	auto body = PhysicsBody::createBox(Size(26, 26), PhysicsMaterial(0, 0, 0));
	this->setPhysicsBody(body);

	body->setContactTestBitmask(1);				// gọi callback với tất cả category
	//body->setRotationEnable(false);

	body->getShapes().at(0)->setSensor(true);	// vẫn gọi callback nhưng ko có tương tác vật lý

	// listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Tank::onContactBegin, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

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


bool Tank::onContactBegin(PhysicsContact & contact)
{
	_velocity = 0;
	this->removeStatus(eStatus::RUNNING);
	
	return true;
}

void Tank::updateWithStatus(eStatus status)
{
	switch (status)
	{
	case DIE:
	{
		if (this->getParent() == nullptr)
			break;

		auto explosion = Explosion::create(false);
		explosion->setPosition(this->getPosition());

		this->getParent()->addChild(explosion);
		this->runAction(RemoveSelf::create());

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
