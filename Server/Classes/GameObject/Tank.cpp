#include "Tank.h"

Tank::Tank(eObjectId id) : GameObject(id),
	_velocity(0)
{
	_direction = eDirection::UP;
	_buffer = new Buffer(25);
}

Tank::Tank(Buffer& data)
{
	_buffer = new Buffer(25);
	this->deserialize(data);
}

Tank::~Tank()
{
	delete _buffer;
}

bool Tank::init()
{
	
	return true;
}

void Tank::update(float dt)
{
	// update postion
	this->updatePosition(dt);
}

void Tank::updatePosition(float dt)
{
	if (!this->hasStatus(eStatus::RUNNING))
	{
		if (_velocity != 0)
		{
			_velocity = 0;
		}
	}
	else
	{
		if (_velocity == 0)
		{
			_velocity = TANK_NORMAL_VELOCITY;
		}
	}

	if (_velocity != 0)
	{
		this->onChanged();
	}

	switch (_direction)
	{
	case LEFT:
		this->setPosition(this->getPosition().x - _velocity * dt, this->getPosition().y);
		break;
	case UP:
		this->setPosition(this->getPosition().x, this->getPosition().y + _velocity * dt);
		break;
	case RIGHT:
		this->setPosition(this->getPosition().x + _velocity * dt, this->getPosition().y);
		break;
	case DOWN:
		this->setPosition(this->getPosition().x, this->getPosition().y - _velocity * dt);
		break;
	default:
		break;
	}
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

void Tank::deserialize(Buffer& data)
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

void Tank::setVelocity(float velocity)
{
	_velocity = velocity;
}

float Tank::getVeloctiy()
{
	return _velocity;
}

void Tank::onChanged()
{
	this->setChanged(true);
}
