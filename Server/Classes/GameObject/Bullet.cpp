#include "Bullet.h"
#include "..\Base\Vector2.h"

Bullet::Bullet(const Vector2 & position, eDirection direction) : GameObject(eObjectId::BULLET)
{
	_position = position;
	_direction = direction;
	_status = eStatus::NORMAL;
	_speed = 500.0f;

	this->init();
}

Bullet::Bullet(Buffer & buffer) : GameObject(buffer)
{
	_speed = 500.0f;

	this->init();
}

Bullet::~Bullet()
{
	delete _collisionChecker;
}

bool Bullet::init()
{
	_boudingBox.width = 6;
	_boudingBox.height = 6;

	this->updateBoudingBox();

	_collisionChecker = new AABB();

	return true;
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

	this->updateBoudingBox();
}

void Bullet::onChanged()
{
	this->setChanged(true);
}

void Bullet::checkCollision(GameObject & object, float dt)
{
	eDirection result;
	float time = _collisionChecker->checkCollision(*this, object, result ,dt);
	if (result != eDirection::NONE)
	{
		this->setStatus(DIE);
		this->onChanged();
		
		object.setStatus(eStatus::DIE);
		object.onChanged();
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

void Bullet::checkPosition()
{
	if (this->getPosition().x >= WINDOW_WIDTH || this->getPosition().x <= 0 || 
		this->getPosition().y >= WINDOW_HEIGHT|| this->getPosition().y <= 0)
	{
		this->setStatus(eStatus::DIE);
		this->onChanged();
	}
}

void Bullet::updateBoudingBox()
{
	_boudingBox.position.x = this->getPosition().x - _boudingBox.width / 2;
	_boudingBox.position.y = this->getPosition().y - _boudingBox.height / 2;
}
