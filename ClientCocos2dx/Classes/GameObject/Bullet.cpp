#include "Bullet.h"
#include "Base\SpriteManager.h"
#include "GameObject\Explosion.h"

#include "..\Server\Classes\Shared\Utils.h"

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

	if (bullet->init())
	{
		bullet->initWithBuffer(buffer);
		bullet->autorelease();
		return bullet;
	}

	CC_SAFE_DELETE(bullet);
	return nullptr;
}

void Bullet::deserialize(Buffer & data)
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
	float x = data.readFloat();
	float y = data.readFloat();

	auto number = data.readFloat();

	data.setBeginRead(0);
}

bool Bullet::init()
{
	this->scheduleUpdate();

	_sprite = Sprite::createWithSpriteFrameName(SpriteManager::getInstance()->getObjectName(eObjectId::BULLET));
	this->addChild(_sprite);

	_speed = BULLET_SPEED_01;

	return true;
}

void Bullet::update(float dt)
{
	GameObject::update(dt);
	
	if (_nextPosition != Vec2::ZERO && !_firstUpdated)
	{
		this->setPositionX(tank::lerp(_nextPosition.x, this->getPositionX(), _speed * dt));
		this->setPositionY(tank::lerp(_nextPosition.y, this->getPositionY(), _speed * dt));

		if (this->getPosition() == _nextPosition)
		{
			if (_status == eStatus::DIE)
			{
				this->explode();
			}
		}
	}
	else
	{
		this->predict(dt);
	}

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
		

		break;
	}
	default:
		break;
	}
}

void Bullet::explode()
{
	if (this->getParent() == nullptr)
		return;

	auto explosion = Explosion::create(false);
	explosion->setPosition(this->getPosition());

	this->getParent()->addChild(explosion);
	this->runAction(RemoveSelf::create());
}