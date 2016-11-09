#include "Bullet.h"
#include "..\Base\Vector2.h"

Bullet::Bullet(const Vector2 & position, eDirection direction) : GameObject(eObjectId::BULLET)
{
	_position = position;
	_speed = 500.0f;
	_direction = direction;
	_status = eStatus::NORMAL;
}

Bullet::~Bullet()
{
}

void Bullet::update(float dt)
{
	if (this->getStatus() == eStatus::DIE)
		return;

	switch (_direction)
	{
	case LEFT:
		this->setPosition(this->getPosition().x - _speed * dt, this->getPosition().y);
		break;
	case UP:
		this->setPosition(this->getPosition().x, this->getPosition().y + _speed * dt);
		break;
	case RIGHT:
		this->setPosition(this->getPosition().x + _speed * dt, this->getPosition().y);
		break;
	case DOWN:
		this->setPosition(this->getPosition().x, this->getPosition().y - _speed * dt);
		break;
	default:
		break;
	}

	this->onChanged();
	this->checkPosition();
}

void Bullet::onChanged()
{
	this->setChanged(true);
}

void Bullet::checkPosition()
{
	if (this->getPosition().x >= WINDOW_WIDTH - 32 || this->getPosition().x <= 32 || 
		this->getPosition().y >= WINDOW_HEIGHT - 32|| this->getPosition().y <= 32)
	{
		this->setStatus(eStatus::DIE);
		this->onChanged();
	}
}
