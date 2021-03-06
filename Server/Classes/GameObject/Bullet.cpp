﻿#include "Bullet.h"
#include "..\Base\Vector2.h"
#include "Tank.h"

Bullet::Bullet(const Vector2 & position, eDirection direction) : GameObject(eObjectId::BULLET)
{
	_position = position;
	_direction = direction;
	_status = eStatus::NORMAL;
	_speed = BULLET_SPEED_01;

	this->init();
}

Bullet::Bullet(Buffer & buffer) : GameObject(buffer)
{
	_speed = BULLET_SPEED_01;

	this->init();
}

Bullet::~Bullet()
{
	delete _collisionChecker;
}

Buffer * Bullet::serialize()
{
	_buffer->setIndex(0);
	_buffer->setBeginRead(0);

	_buffer->writeInt(eDataType::OBJECT);
	_buffer->writeInt(this->getId());
	_buffer->writeInt(this->getUniqueId());
	_buffer->writeInt(this->getStatus());
	_buffer->writeByte(this->getDirection());
	_buffer->writeFloat(this->getPosition().x);
	_buffer->writeFloat(this->getPosition().y);
	_buffer->writeInt(_ownerTag);

	return _buffer;
}

void Bullet::deserialize(Buffer & data)
{
	data.setBeginRead(0);

	eDataType type = (eDataType)data.readInt();
	if (type != eDataType::OBJECT)
		return;

	this->setType(type);
	this->setId((eObjectId)data.readInt());
	this->setUniqueId(data.readInt());
	this->setStatus((eStatus)data.readInt());
	this->setDirection((eDirection)data.readByte());
	float x = data.readFloat();
	float y = data.readFloat();
	this->setPosition(x, y);
	_ownerTag = data.readInt();

	data.setBeginRead(0);
}

void Bullet::updateWithTankLevel(eTankLevel tanklevel)
{
	switch (tanklevel)
	{
		case DEFAULT_TANK:
			_damageValue = 1;
			_level = eBulletLevel::NORMAL_BULLET;
			break;
		case BASIC_TANK:
			_damageValue = 1;
			_level = eBulletLevel::SLOW_BULLET;
			break;
		case SECOND_TANK:
		case FAST_TANK:
			_damageValue = 1;
			_level = eBulletLevel::NORMAL_BULLET;
			break;
		case THIRD_TANK:
		case POWER_TANK:
			_damageValue = 1;
			_level = eBulletLevel::FAST_BULLET;
			break;
			break;
		case FOURTH_TANK:
		case ARMOR_TANK:
			_damageValue = 2;
			_level = eBulletLevel::NORMAL_BULLET;
			break;
		default:
			break;
	}

	this->getSpeed();
}

float Bullet::getSpeed()
{
	switch (_level)
	{
	case SLOW_BULLET:
		_speed = BULLET_SPEED_01;
		break;
	case NORMAL_BULLET:
		_speed = BULLET_SPEED_02;
		break;
	case FAST_BULLET:
		_speed = BULLET_SPEED_03;
		break;
	default:
		_speed = BULLET_SPEED_02;
		break;
	}

	return _speed;
}

bool Bullet::init()
{
	_boudingBox.width = 6;
	_boudingBox.height = 6;

	this->updateBoudingBox();

	_collisionChecker = new AABB();

	_damageValue = 1;
	_damagedObjectCounter = 0;
	_level = eBulletLevel::NORMAL_BULLET;

	return true;
}

void Bullet::update(float dt)
{
	if (this->getStatus() == eStatus::DIE)
		return;

	switch (_direction)
	{
	case LEFT:
		this->setPosition(this->getPosition().x - this->getSpeed() * dt, this->getPosition().y);
		break;
	case UP:
		this->setPosition(this->getPosition().x, this->getPosition().y + this->getSpeed() * dt);
		break;
	case RIGHT:
		this->setPosition(this->getPosition().x + this->getSpeed() * dt, this->getPosition().y);
		break;
	case DOWN:
		this->setPosition(this->getPosition().x, this->getPosition().y - this->getSpeed() * dt);
		break;
	default:
		break;
	}

	this->onChanged();
	this->checkPosition();

	this->updateBoudingBox();
}

void Bullet::onChanged()
{
	this->setChanged(true);
}

void Bullet::checkCollision(GameObject & object, float dt)
{
	if (object.getId() == eObjectId::GRASS_WALL || object.getId() == eObjectId::WATER_WALL ||
		object.hasStatus(eStatus::PROTECTED))
		return;

	if (_owner == nullptr)
		return;

	if (_owner == &object || _damagedObjectCounter >= 2 || _owner->getId() == object.getId())
		return;

	eDirection result;
	float time = _collisionChecker->checkCollision(*this, object, result, dt);

	if (result != eDirection::NONE)
	{
		this->explode();
		
		auto damage = Damage::create(_owner->getId(), _damageValue, _direction);
		damage->setLevel(_level);

		object.gotHit(damage);

		_damagedObjectCounter++;
	}
}

Vector2 Bullet::getVelocity() const
{
	switch (_direction)
	{
	case LEFT:
		return Vector2(-_speed, 0.0f);
	case UP:
		return Vector2(0.0f, _speed);
	case RIGHT:
		return Vector2(_speed, 0.0f);
	case DOWN:
		return Vector2(0.0f, -_speed);
	default:
		break;
	}

	return Vector2(0.0f, 0.0f);
}

void Bullet::setOwner(GameObject * owner)
{
	_owner = owner;
	if (owner == nullptr)
		return;

	_ownerTag = owner->getUniqueId();
}

GameObject * Bullet::getOwner()
{
	return _owner;
}

void Bullet::setDamageValue(int value)
{
	_damageValue = value;
}

int Bullet::getDamageValue()
{
	return _damageValue;
}

void Bullet::checkPosition()
{
	if (this->getPosition().x >= 26 * TILE_WIDTH || this->getPosition().x <= 0 || 
		this->getPosition().y >= WINDOW_HEIGHT|| this->getPosition().y <= 0)
	{
		this->explode();
	}
}

void Bullet::updateBoudingBox()
{
	_boudingBox.position.x = this->getPosition().x - _boudingBox.width / 2;
	_boudingBox.position.y = this->getPosition().y - _boudingBox.height / 2;
}

void Bullet::explode()
{
	if (_status == eStatus::DIE)
		return;

	this->setStatus(DIE);
	this->onChanged();

	if (_owner != nullptr)
	{
		auto tank = dynamic_cast<Tank*>(_owner);
		if (tank)
		{
			tank->setNumberOfBullets(tank->getNumberOfBullets() - 1);
			tank->removeBullet(this->getUniqueId());
		}
	}
}
