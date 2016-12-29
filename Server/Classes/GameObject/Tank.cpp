#include "Tank.h"
#include "Bullet.h"
#include "..\Definitions.h"
#include "..\Base\AABB.h"
#include "..\Base\SceneManager.h"

#include "..\Shared\Utils.h"
#include "..\Shared\Converter.h"

Tank::Tank(eObjectId id) : GameObject(id),
	_velocity(0),
	_tankLevel(eTankLevel::BASIC_TANK),
	_startPositionIndex(0)
{
	this->init();
}

Tank::Tank(Buffer& data) : GameObject(data),
	_velocity(0),
	_startPositionIndex(0)
{
	this->init();
}

Tank::~Tank()
{
	delete _collisionChecker;
}

Buffer * Tank::serialize()
{
	_buffer->setIndex(0);
	_buffer->setBeginRead(0);

	_buffer->writeInt(eDataType::TANK);
	_buffer->writeInt(this->getId());
	_buffer->writeInt(this->getUniqueId());
	_buffer->writeInt(this->getStatus());
	_buffer->writeByte(this->getDirection());
	_buffer->writeFloat(this->getPosition().x);
	_buffer->writeFloat(this->getPosition().y);
	_buffer->writeInt(this->getTankLevel());
	
	_buffer->writeInt(this->getStartPositionIndex());

	return _buffer;
}

void Tank::deserialize(Buffer & data)
{
	data.setBeginRead(0);

	eDataType type = (eDataType)data.readInt();
	if (type != eDataType::TANK)
		return;

	this->setType(type);
	this->setId((eObjectId)data.readInt());
	this->setUniqueId(data.readInt());
	this->setStatus((eStatus)data.readInt());
	this->setDirection((eDirection)data.readByte());
	float x = data.readFloat();
	float y = data.readFloat();
	this->setPosition(x, y);
	this->setTankLevel((eTankLevel)data.readInt());
	this->setStartPositionIndex(data.readInt());

	data.setBeginRead(0);
}

bool Tank::init()
{
	_buffer = new Buffer(BUFFER_SIZE_TANK);

	_boudingBox.width = 32;
	_boudingBox.height = 32;

	this->updateBoundingBoxPosition();

	_collisionChecker = new AABB();
	_collisionChecker->setOffset(2);

	this->setTankLevel(eTankLevel::DEFAULT_TANK);

	_isActive = true;
	_health = this->getMaxHealth();
	
	return true;
}

void Tank::update(float dt)
{
	if (!_isActive)
		return;

	this->updatePosition(dt);

	this->updateBoundingBoxPosition();
	this->resetCollidingSide();

	this->fixWithBounding();
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
			_velocity = this->getVelocityByLevel();
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

int Tank::getMaxHealth()
{
	switch (_tankLevel)
	{
	case eTankLevel::BASIC_TANK:
		return 1;
	case eTankLevel::FAST_TANK:
		return 1;
	case eTankLevel::POWER_TANK:
		return 1;
	case eTankLevel::ARMOR_TANK:
		return 3;
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

float Tank::getVelocityByLevel()
{
	switch (_tankLevel)
	{
	case DEFAULT_TANK:
		return TANK_NORMAL_VELOCITY;
	case SECOND_TANK:
		return TANK_NORMAL_VELOCITY;
	case THIRD_TANK:
		return TANK_FAST_VELOCITY;
	case FOURTH_TANK:
		return TANK_FAST_VELOCITY;
	case BASIC_TANK:
		return TANK_SLOW_VELOCITY;
	case FAST_TANK:
		return TANK_FAST_VELOCITY;
	case POWER_TANK:
		return TANK_NORMAL_VELOCITY;
	case ARMOR_TANK:
		return TANK_NORMAL_VELOCITY;
	default:
		return TANK_NORMAL_VELOCITY;
	}
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
	if (!_isActive)
		return;

	if (other.getId() == eObjectId::GRASS_WALL || other.getId() == eObjectId::BULLET)
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

void Tank::resetCollidingSide()
{
	// reset hướng va chạm
	for (auto it = _objectCollidingCounter.begin(); it != _objectCollidingCounter.end(); it++)
	{
		it->second = 0;
	}

	_collidingSide = eDirection::NONE;
}

void Tank::move(eDirection direction, float dt)
{
	this->setDirection(direction);

	this->addStatus(eStatus::RUNNING);
	_velocity = this->getVelocityByLevel();

	// fix position for turning
	// this->fixPositionForTurn();

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
	auto delta = newPos - this->getPosition();
	
	if (delta.lenght() < TILE_WIDTH)
	{
		this->setPosition(newPos);
	}

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

void Tank::setTankLevel(eTankLevel level)
{
	_tankLevel = level;
}

eTankLevel Tank::getTankLevel()
{
	return _tankLevel;
}

unsigned int Tank::getBufferSize()
{
	return BUFFER_SIZE_TANK;
}

void Tank::setStartPositionIndex(int index)
{
	_startPositionIndex = index;
}

int Tank::getStartPositionIndex()
{
	return _startPositionIndex;
}

void Tank::gotHit(Damage * damage)
{
	_health -= damage->getValue();

	if (_health <= 0)
	{
		this->setStatus(eStatus::DIE);
		this->onChanged();
	}

	delete damage;
}

void Tank::fixWithBounding()
{
	//
	if (this->getPosition().x < TANK_SIZE_WIDTH / 2)
	{
		this->setPosition(TANK_SIZE_WIDTH / 2, this->getPosition().y);
	}

	if (this->getPosition().x > 26 * TILE_WIDTH - TANK_SIZE_WIDTH / 2)
	{
		this->setPosition(26 * TILE_WIDTH - TANK_SIZE_WIDTH / 2, this->getPosition().y);
	}

	if (this->getPosition().y < TANK_SIZE_WIDTH / 2)
	{
		this->setPosition(this->getPosition().x, TANK_SIZE_WIDTH / 2);
	}

	if (this->getPosition().y > 32 * TILE_WIDTH - TANK_SIZE_WIDTH / 2)
	{
		this->setPosition(this->getPosition().x, 32 * TILE_WIDTH - TANK_SIZE_WIDTH / 2);
	}
}
