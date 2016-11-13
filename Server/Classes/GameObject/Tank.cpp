#include "Tank.h"

Tank::Tank(eObjectId id) : GameObject(id),
	_velocity(0)
{
	this->init();
}

Tank::Tank(Buffer& data) : GameObject(data),
	_velocity(0)
{
	this->init();
}

Tank::~Tank()
{
}

bool Tank::init()
{
	_boudingBox.position.x = this->getPosition().x;
	_boudingBox.position.y = this->getPosition().y;
	_boudingBox.width = 26;
	_boudingBox.height = 26;

	_collisionChecker = new AABB();

	return true;
}

void Tank::update(float dt)
{
	// update postion
	this->updatePosition(dt);

	// update bounding box
	_boudingBox.position.x = this->getPosition().x;
	_boudingBox.position.y = this->getPosition().y;
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

void Tank::updatePosition(float dt, float distance)
{
	if (distance != 0)
	{
		this->onChanged();
	}

	switch (_direction)
	{
	case LEFT:
		this->setPosition(this->getPosition().x - distance, this->getPosition().y);
		break;
	case UP:
		this->setPosition(this->getPosition().x, this->getPosition().y + distance);
		break;
	case RIGHT:
		this->setPosition(this->getPosition().x + distance, this->getPosition().y);
		break;
	case DOWN:
		this->setPosition(this->getPosition().x, this->getPosition().y - distance);
		break;
	default:
		break;
	}
}

void Tank::setVelocity(float velocity)
{
	_velocity = velocity;
}

float Tank::getVeloctiy()
{
	return _velocity;
}

Vector2 Tank::getVelocity() const
{
	switch (_direction)
	{
	case LEFT:
		return Vector2(-_velocity, 0.0f);
	case UP:
		return Vector2(0.0f, _velocity);
	case RIGHT:
		return Vector2(_velocity, 0.0f);
	case DOWN:
		return Vector2(0.0f, -_velocity);
	default:
		break;
	}
	
	return Vector2(0.0f, 0.0f);
}

void Tank::onChanged()
{
	this->setChanged(true);
}

void Tank::checkCollision(GameObject & other, float dt)
{
	eDirection result;
	float time = _collisionChecker->checkCollision(*this, other, result, dt);

	// có va chạm
	if (result != eDirection::NONE)
	{
		if (time < 1.0f)
		{
			float distance = (_velocity * dt) * time;
			this->updatePosition(dt, distance);

			// ko cho chạy nữa
			this->removeStatus(eStatus::RUNNING);

			this->onChanged();
		}
	}

	float moveX, moveY;
	if (_collisionChecker->isColliding(this->getBoundingBox(), other.getBoundingBox(), moveX, moveY, dt))
	{
		this->setPosition(this->getPosition().x + moveX, this->getPosition().y + moveY);
		this->onChanged();
	}
}
