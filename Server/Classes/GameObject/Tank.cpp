#include "Tank.h"
#include "Bullet.h"
#include "..\Base\AABB.h"
#include "..\Base\SceneManager.h"

#include "..\Shared\Utils.h"

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

	_inputTurns.push_back(_direction);

	_remainMoveForTurn = 0.0f;

	return true;
}

void Tank::update(float dt)
{
	this->turnWithInputQueue(dt);

	this->updateWithInputQueue(dt);

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

			_inputTurns.clear();
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

void Tank::updatePosition(float dt, float distance)
{
	if (distance != 0)
	{
		this->onChanged();
	}

	this->move(distance);
}

void Tank::updateBoundingBoxPosition()
{
	// update bounding box
	_boudingBox.position.x = this->getPosition().x - _boudingBox.width / 2;
	_boudingBox.position.y = this->getPosition().y - _boudingBox.height / 2;
}

void Tank::turnWithInputQueue(float dt)
{
	if (_inputTurns.size() <= 0 || _remainMoveForTurn > 0)
		return;

	eDirection direction = _inputTurns.back();

	if (direction == eDirection::NONE || _direction == direction)
	{
		return;
	}

	// nếu mà ko có va chạm hướng đó hoặc va chạm hơn 2 cái thì update direction luôn
	if (!this->isCollidingAtSide(direction) || _objectCollidingCounter.at(direction) > 1)
	{
		this->updateDirection(direction);
		return;
	}

	auto position = this->getPosition();
	
	int unit = 16;
	float integral = 0.0f;
	float fractionalX = 0.0f;	// phần lẻ sau dấu phẩy của vị trí ban đầu
	fractionalX = modf(position.x, &integral);
	float remainX = unit - (((int)integral % unit) + fractionalX);

	float integralY = 0.0f;
	float fractionalY = 0.0f;
	fractionalY = modf(position.y, &integralY);
	float remainY = unit - (((int)integralY % unit) + fractionalY);

	if (fractionalX < fractionalY)
	{
		_remainMoveForTurn = remainX;
	}
	else
	{
		_remainMoveForTurn = remainY;
	}

	_inputTurns.clear();
	_inputTurns.push_back(direction);
	_inputTurns.push_back(_direction);

	// printf("begin remain: %.2f, current dir: %d, dir: %d\n", _remainMoveForTurn, _direction, direction);

	return;
}

void Tank::updateWithInputQueue(float dt)
{
	auto direction = this->getDirectionInQueue();
	this->updateDirection(direction);
}

void Tank::generateInput(eDirection direction, int number)
{
	for (int i = 0; i < number; i++)
	{
		_inputTurns.push_back(direction);
	}
}

void Tank::move(float distance)
{
	if (this->isCollidingAtSide(_direction) || distance == 0)
		return;

	if (_remainMoveForTurn > distance)
	{
		_remainMoveForTurn -= distance;
		printf("remain: %.2f\n", _remainMoveForTurn);
	}
	else if (_remainMoveForTurn > 0.0f)
	{
		distance = _remainMoveForTurn;
		_remainMoveForTurn = 0.0f;
		printf("end: %.2f\n", _remainMoveForTurn);
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

eDirection Tank::getDirectionInQueue()
{
	if (_inputTurns.size() == 0)
		return _direction;

	auto ret = _inputTurns.back();

	if(_remainMoveForTurn == 0)
		_inputTurns.pop_back();

	return ret;
}

void Tank::updateDirection(eDirection direction)
{
	if (_direction == direction)
		return;

	_direction = direction;
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
		this->updatePosition(dt, distance);
		this->onContactBegin(other);

		// ko cho chạy nữa
		if (_inputTurns.size() == 0)
		{
			this->removeStatus(eStatus::RUNNING);
		}

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
	if (_inputTurns.empty())
	{
		_inputTurns.push_back(direction);
	}
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

	this->move(_velocity * dt);
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
