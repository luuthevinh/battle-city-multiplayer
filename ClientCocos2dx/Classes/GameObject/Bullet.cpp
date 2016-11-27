#include "Bullet.h"
#include "Base\SpriteManager.h"
#include "GameObject\Explosion.h"

Bullet::Bullet() : GameObject(eObjectId::BULLET),
	_direction(eDirection::UP)
{
}

Bullet::~Bullet()
{
}

Bullet * Bullet::create(const Vec2 & position, eDirection direction)
{
	Bullet* bullet = new(std::nothrow) Bullet();
	if (bullet->init())
	{
		bullet->autorelease();
		bullet->setPosition(position);
		bullet->setDirection(direction);
		return bullet;
	}

	CC_SAFE_DELETE(bullet);
	return nullptr;
}

Bullet * Bullet::createWithBuffer(Buffer & buffer)
{
	Bullet* bullet = new(std::nothrow) Bullet();
	bullet->deserialize(buffer);

	if (bullet->init())
	{
		bullet->autorelease();
		return bullet;
	}

	CC_SAFE_DELETE(bullet);
	return nullptr;
}

bool Bullet::init()
{
	this->scheduleUpdate();

	_sprite = Sprite::createWithSpriteFrameName(SpriteManager::getInstance()->getObjectName(eObjectId::BULLET));
	this->addChild(_sprite);

	_speed = 200.0f;


	return true;
}

void Bullet::update(float dt)
{
	GameObject::update(dt);

	// update sprite rotate
	switch (_direction)
	{
	case LEFT:
		this->setRotation(0.0f);
		break;
	case UP:
		this->setRotation(90.0f);
		break;
	case RIGHT:
		this->setRotation(180.0f);
		break;
	case DOWN:
		this->setRotation(-90.0f);
		break;
	default:
		break;
	}
}

void Bullet::predict(float dt)
{
	switch (_direction)
	{
	case LEFT:
		this->setPositionX(this->getPositionX() - _speed * dt);
		this->setRotation(0.0f);
		break;
	case UP:
		this->setPositionY(this->getPositionY() + _speed * dt);
		this->setRotation(90.0f);
		break;
	case RIGHT:
		this->setPositionX(this->getPositionX() + _speed * dt);
		this->setRotation(180.0f);
		break;
	case DOWN:
		this->setPositionY(this->getPositionY() - _speed * dt);
		this->setRotation(-90.0f);
		break;
	default:
		break;
	}
}

void Bullet::setDirection(eDirection direction)
{
	_direction = direction;
}

eDirection Bullet::getDirection()
{
	return _direction;
}

void Bullet::updateWithStatus(eStatus status)
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
	default:
		break;
	}
}