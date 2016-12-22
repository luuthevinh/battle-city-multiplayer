#include "Tank.h"
#include "Bullet.h"
#include "..\Base\AABB.h"
#include "..\Base\SceneManager.h"

#include "..\Shared\Utils.h"
#include "..\Shared\Converter.h"

Tank::Tank(eObjectId id) : GameObject(id),
	_velocity(0),
	_tankLevel(eTankLevel::BASIC_TANK)
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
	delete _collisionChecker;
}

bool Tank::init()
{
	_boudingBox.width = 32;
	_boudingBox.height = 32;

	this->updateBoundingBoxPosition();

	_collisionChecker = new AABB();
	_collisionChecker->setOffset(2);

	return true;
}

void Tank::update(float dt)
{
	this->updatePosition(dt);

	this->updateBoundingBoxPosition();

	// reset hướng va chạm
	for (auto it = _objectCollidingCounter.begin(); it != _objectCollidingCounter.end(); it++)
	{
		it->second = 0;
	}

	_collidingSide = eDirection::NONE;
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

	if (!_commandQueue.empty())
	{
		this->updateWithCommand(_commandQueue.front(), dt);

		if (_commandQueue.size() == 1 && _commandQueue.front()->begin &&  _commandQueue.front()->input != eKeyInput::KEY_SHOOT)
		{
			return;
		}

		delete _commandQueue.front();
		_commandQueue.pop();
	}

	if (_velocity != 0)
	{
		this->onChanged();
	}
}

void Tank::updateBoundingBoxPosition()
{
	// update bounding box
	_boudingBox.position.x = this->getPosition().x - _boudingBox.width / 2;
	_boudingBox.position.y = this->getPosition().y - _boudingBox.height / 2;
}

void Tank::moveByDistance(float distance)
{
	if (this->isCollidingAtSide(_direction) || distance == 0)
		return;

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

void Tank::checkCollidingSide(GameObject& other)
{
	auto otherRect = other.getBoundingBox();

	auto side = _collisionChecker->getSideOverlap(this->getBoundingBox(), otherRect);

	if (side != eDirection::NONE)
	{
		_objectCollidingCounter[side]++;
	}

	_collidingSide = (eDirection)(_collidingSide | side);
}

bool Tank::isCollidingAtSide(eDirection side)
{
	return (_collidingSide & side) == side;
}

int Tank::getMaxBullet()
{
	switch (_tankLevel)
	{
	case eTankLevel::BASIC_TANK:
		return 1;
	case eTankLevel::FAST_TANK:
		return 2;
	case eTankLevel::POWER_TANK:
		return 3;
	case eTankLevel::ARMOR_TANK:
		return 2;
	default:
		break;
	}

	return 1;
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
	if (!this->canCollisionWith(other.getCategoryBitmask()))
		return;

	if (other.getId() == eObjectId::BULLET)
	{
		return;
	}

	this->updateBoundingBoxPosition();

	this->checkCollidingSide(other);

	eDirection result;
	float time = _collisionChecker->checkCollision(*this, other, result, dt);

	// có va chạm
	if (time < 1.0f)
	{
		float distance = (_velocity * dt) * time;

		this->moveByDistance(distance);

		this->removeStatus(eStatus::RUNNING);
		
		this->onContactBegin(other);
		this->onChanged();
	}
	
	float moveX, moveY;

	if (_collisionChecker->isColliding(this->getBoundingBox(), other.getBoundingBox(), moveX, moveY, dt))
	{
		this->setPosition(this->getPosition().x + moveX, this->getPosition().y + moveY);
	}
}

void Tank::setDirection(eDirection direction)
{
	if (_direction != direction)
	{
		_oldDirection = _direction;
	}

	_direction = direction;
}

void Tank::setNumberOfBullets(int number)
{
	_bulletCounter = number;
}

int Tank::getNumberOfBullets()
{
	return _bulletCounter;
}

void Tank::handleData(Serializable * data)
{
	if (data->getType() == eDataType::OBJECT)
	{
		this->deserialize(*(data->serialize()));
		this->onChanged();
	}
}

void Tank::updateWithCommand(CommandPacket* commad, float dt)
{
	auto key = commad->input;
	if (commad->begin)
	{
		switch (key)
		{
		case KEY_NONE:
			break;
		case KEY_LEFT:
			this->move(eDirection::LEFT, dt);
			break;
		case KEY_RIGHT:
			this->move(eDirection::RIGHT, dt);
			break;
		case KEY_UP:
			this->move(eDirection::UP, dt);
			break;
		case KEY_DOWN:
			this->move(eDirection::DOWN, dt);
			break;
		case KEY_SHOOT:
			this->shoot();
			break;
		default:
			break;
		}
	}
	else
	{
		this->removeStatus(eStatus::RUNNING);
		_velocity = 0;
		this->onChanged();
	}
}

void Tank::move(eDirection direction, float dt)
{
	this->setDirection(direction);

	this->addStatus(eStatus::RUNNING);
	_velocity = TANK_NORMAL_VELOCITY;

	// fix position for turning
	this->fixPositionForTurn();

	this->moveByDistance(_velocity * dt);
}

void Tank::fixPositionForTurn()
{
	if (_oldDirection == _direction)
		return;

	if (!this->isCollidingAtSide(_direction) || _objectCollidingCounter.at(_direction) > 1)
		return;

	switch (_oldDirection)
	{
	case LEFT:
	case RIGHT:
	{
		if (_direction == eDirection::LEFT || _direction == eDirection::RIGHT)
			return;
		break;
	}
	case UP:
	case DOWN:
	{
		if (_direction == eDirection::UP || _direction == eDirection::DOWN)
			return;
		break;
	}
	default:
		break;
	}

	auto position = this->getPosition();

	// lúc này đã bỏ phần dư rồi, nên lúc sau ko cần trừ nữa
	auto x = (int)(position.x) / TILE_WIDTH;
	auto y = (int)(position.y) / TILE_WIDTH;

	switch (_oldDirection)
	{
	case eDirection::UP:
	{
		if ((int)(position.y) % TILE_WIDTH != 0)
		{
			y++;
		}
		break;
	}
	case eDirection::RIGHT:
	{
		if ((int)(position.x) % TILE_WIDTH != 0)
		{
			x++;
		}
		break;
	}
	case eDirection::DOWN:
		break;
	case eDirection::LEFT:
		break;
	default:
		break;
	}

	auto newPos = Vector2(TILE_WIDTH * (x), TILE_WIDTH * (y));
	this->setPosition(newPos);

	_oldDirection = _direction;
}


void Tank::shoot()
{
	if (_bulletCounter >= this->getMaxBullet())
	{
		return;
	}

	Vector2 shootPosition = this->getPosition();
	float offset = 0;

	switch (_direction)
	{
	case LEFT:
		shootPosition.x -= offset;
		break;
	case UP:
		shootPosition.y += offset;
		break;
	case RIGHT:
		shootPosition.x += offset;
		break;
	case DOWN:
		shootPosition.y -= offset;
		break;
	default:
		break;
	}

	auto bullet = new Bullet(shootPosition, this->getDirection());
	bullet->setOwner(this);

	SceneManager::getInstance()->getCurrentScene()->addObject(bullet);

	_bulletCounter++;

	//printf("shoot: %.2f, %.2f\n", bullet->getPosition().x, bullet->getPosition().y);
}

void Tank::onContactBegin(GameObject& object)
{
}
