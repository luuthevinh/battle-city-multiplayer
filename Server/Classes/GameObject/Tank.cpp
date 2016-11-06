#include "Tank.h"

Tank::Tank(eObjectId id) : GameObject(id),
	_velocity(0),
	_direction(eDirection::UP)
{
	_packet.packetType = Packet::TANK;
	_packet.fromSocket = 0;
}

Tank::~Tank()
{
}

bool Tank::init()
{
	
	return true;
}

void Tank::update(float dt)
{
	// update postion
	this->updatePosition(dt);

	// this->onChanged();
}

void Tank::updatePosition(float dt)
{
	if (!this->hasStatus(eStatus::RUNNING))
	{
		if (_velocity != 0)
		{
			_velocity = 0;
			 //this->onChanged();
		}
	}
	else
	{
		if (_velocity == 0)
		{
			_velocity = TANK_NORMAL_VELOCITY;
			 //this->onChanged();
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

void Tank::setDirection(eDirection direction)
{
	if (_direction == direction || direction <= 0 || direction > 4)
		return;

	_direction = direction;
}

eDirection Tank::getDirection()
{
	return _direction;
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
	if (_packet.TankPacket.direction != this->getDirection() || 
		_packet.TankPacket.x != this->getPosition().x ||
		_packet.TankPacket.y != this->getPosition().y ||
		_packet.TankPacket.status != this->getStatus())
	{
		this->setChanged(true);

		_packet.TankPacket.direction = this->getDirection();
		_packet.TankPacket.x = this->getPosition().x;
		_packet.TankPacket.y = this->getPosition().y;
		_packet.TankPacket.status = this->getStatus();
		_packet.TankPacket.id = this->getTag();
	}
}
