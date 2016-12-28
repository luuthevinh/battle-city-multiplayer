#include "Tank.h"
#include "Explosion.h"
#include "Base\ServerConnector.h"
#include "Bullet.h"

// shared
#include "..\Server\Classes\Shared\Buffer.h"
#include "..\Server\Classes\Shared\Utils.h"
#include "..\Server\Classes\Shared\Converter.h"

Tank::Tank(eObjectId id) :
	_velocity(0)
{
	this->createBuffer();
	this->setName(SpriteManager::getInstance()->getObjectName(id));

	_previousBuffer = new Buffer(_buffer->getSize());
	_lastBuffer = this->serialize()->clone();
	_firstUpdated = true;

	_tankLevel = eTankLevel::DEFAULT_TANK;
}

Tank::Tank(Buffer& data) :
	_velocity(0)
{

}

Tank::~Tank()
{
	for (auto i = _animations.begin(); i != _animations.end(); i++)
	{
		i->second->release();
	}
}

Tank* Tank::create(eObjectId id)
{
	Tank* tank = new(std::nothrow) Tank(id);
	if (tank && tank->init())
	{
		tank->autorelease();
		return tank;
	}
	else
	{
		delete tank;
		tank = nullptr;
		return nullptr;
	}
}

Tank * Tank::createWithBuffer(Buffer &data)
{
	Tank* tank = new(std::nothrow) Tank(data);

	if (tank && tank->init())
	{
		tank->initWithBuffer(data);
		tank->autorelease();
		return tank;
	}
	else
	{
		delete tank;
		tank = nullptr;
		return nullptr;
	}
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
	_buffer->writeFloat(ServerConnector::getInstance()->getTime());

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
	this->setTag(data.readInt());
	this->setStatus((eStatus)data.readInt());
	this->setDirection((eDirection)data.readByte());
	float x = data.readFloat();
	float y = data.readFloat();
	this->setTankLevel((eTankLevel)data.readInt());

	auto number = data.readFloat();

	data.setBeginRead(0);
}

void Tank::createBuffer()
{
	_buffer = new Buffer(BUFFER_SIZE_TANK);
}

void Tank::setTankLevel(eTankLevel level)
{
	_tankLevel = level;
	auto s = tank::Converter::tankLevelToString(_tankLevel);
	
	CCLOG("tank level: %s", s);

	this->updateSprite();
}

eTankLevel Tank::getTankLevel()
{
	return _tankLevel;
}

unsigned int Tank::getBufferSize()
{
	return BUFFER_SIZE_TANK;
}

bool Tank::init()
{
	this->setZOrder(TANK_Z_INDEX);

	// update object
	this->scheduleUpdate();

	_sprite = Sprite::create();
	this->addChild(_sprite);

	auto level = tank::Converter::tankLevelToString(_tankLevel);

	_animations[eDirection::UP] = SpriteManager::getInstance()->getAnimate(this->getName() + level + "_up");
	_animations[eDirection::DOWN] = SpriteManager::getInstance()->getAnimate(this->getName() + level + "_down");
	_animations[eDirection::LEFT] = SpriteManager::getInstance()->getAnimate(this->getName() + level + "_left");
	_animations[eDirection::RIGHT] = SpriteManager::getInstance()->getAnimate(this->getName() + level + "_right");

	for (auto i = _animations.begin(); i != _animations.end(); i++)
	{
		if(i->second != nullptr)
			i->second->retain();
	}

	this->runAnimateByDirection(eDirection::UP);

	auto body = PhysicsBody::createBox(Size(32, 32), PhysicsMaterial(0, 0, 0));
	this->setPhysicsBody(body);

	body->setContactTestBitmask(eObjectId::ALL);			// gọi callback với tất cả category
	body->getShapes().at(0)->setSensor(true);	// vẫn gọi callback nhưng ko có tương tác vật lý

	// listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Tank::onContactBegin, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(Tank::onContactSeparate, this);
	contactListener->onContactPreSolve = CC_CALLBACK_2(Tank::onPreSolve, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->setContentSize(Size(32.0f, 32.0f));

	return true;
}

void Tank::update(float dt)
{
	GameObject::update(dt);

	if(!_firstUpdated)
	{
		this->setPositionX(tank::lerp(_nextPosition.x, this->getPositionX(), this->getVelocityByLevel() * dt));
		this->setPositionY(tank::lerp(_nextPosition.y, this->getPositionY(), this->getVelocityByLevel() * dt));
	}
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

	if (_velocity == 0)
		return;

	switch (_direction)
	{
	case LEFT:
		this->setPositionX(this->getPositionX() - _velocity * dt);
		break;
	case UP:
		this->setPositionY(this->getPositionY() + _velocity * dt);
		break;
	case RIGHT:
		this->setPositionX(this->getPositionX() + _velocity * dt);
		break;
	case DOWN:
		this->setPositionY(this->getPositionY() - _velocity * dt);
		break;
	default:
		break;
	}

	this->onChanged();
}

void Tank::setId(eObjectId id)
{
	if (_id != id)
	{
		GameObject::setId(id);
		this->updateSprite();
	}

}

void Tank::runAnimateByDirection(eDirection direction)
{
	if (direction <= 0 || direction > 8 || _sprite == nullptr)
		return;

	if (_animations[direction] == nullptr)
		return;

	_sprite->stopAllActions();

	if (this->hasStatus(eStatus::RUNNING))
	{
		_sprite->runAction(RepeatForever::create(_animations[direction]));
	}
	else
	{
		_sprite->runAction(_animations[direction]);
	}
}

void Tank::updateSprite()
{
	// xóa cái cũ đi
	for (auto i = _animations.begin(); i != _animations.end(); i++)
	{
		if(i->second != nullptr)
			i->second->release();
	}
	
	auto idName = SpriteManager::getInstance()->getObjectName(this->getId());
	auto level = tank::Converter::tankLevelToString(_tankLevel);

	_animations[eDirection::UP] = SpriteManager::getInstance()->getAnimate(idName + level + "_up");
	_animations[eDirection::DOWN] = SpriteManager::getInstance()->getAnimate(idName + level + "_down");
	_animations[eDirection::LEFT] = SpriteManager::getInstance()->getAnimate(idName + level + "_left");
	_animations[eDirection::RIGHT] = SpriteManager::getInstance()->getAnimate(idName + level + "_right");

	for (auto i = _animations.begin(); i != _animations.end(); i++)
	{
		if(i->second != nullptr)
			i->second->retain();
	}
}

void Tank::updateWithCommand(CommandPacket * commad, float dt)
{
	auto key = commad->input;

	if (commad->begin)
	{
		switch (key)
		{
		case eKeyInput::KEY_LEFT:
		{
			this->move(eDirection::LEFT, dt);
			break;
		}
		case eKeyInput::KEY_RIGHT:
		{
			this->move(eDirection::RIGHT, dt);
			break;
		}
		case eKeyInput::KEY_DOWN:
		{
			this->move(eDirection::DOWN, dt);
			break;
		}
		case eKeyInput::KEY_UP:
		{
			this->move(eDirection::UP, dt);
			break;
		}
		case eKeyInput::KEY_SHOOT:
		{
			this->shoot();
			break;
		}
		default:
			break;
		}
	}
	else
	{
		if (commad->input != eKeyInput::KEY_SHOOT)
		{
			this->removeStatus(eStatus::RUNNING);
		}
	}
}

void Tank::move(eDirection direction, float dt)
{
	this->setDirection(direction);

	if (this->isCollidingAtSide(direction))
		return;

	if (!this->hasStatus(eStatus::RUNNING))
		return;

	// this->fixPositionForTurn();

	switch (_direction)
	{
	case LEFT:
		this->setPositionX(this->getPositionX() - _velocity * dt);
		break;
	case UP:
		this->setPositionY(this->getPositionY() + _velocity * dt);
		break;
	case RIGHT:
		this->setPositionX(this->getPositionX() + _velocity * dt);
		break;
	case DOWN:
		this->setPositionY(this->getPositionY() - _velocity * dt);
		break;
	default:
		break;
	}

	// CCLOG("tank move %.2f/frame", _velocity * dt);

	this->onChanged();
}

void Tank::setDirection(eDirection direction)
{
	if (_direction == direction)
		return;

	_oldDirection = _direction;
	_direction = direction;

	this->runAnimateByDirection(direction);

}


bool Tank::onContactBegin(PhysicsContact & contact)
{
	auto objectA = (GameObject*)contact.getShapeA()->getBody()->getNode();
	auto objectB = (GameObject*)contact.getShapeB()->getBody()->getNode();

	if (objectA->getId() == objectB->getId())
		return true;
	
	if (objectA == this)
	{
		eDirection side = this->getIntersectSide(objectB->getBoundingBox());
		CCLOG("intersect side: %s", tank::Converter::directionToString(side));

		if (side != eDirection::NONE && side == _direction)
		{
			this->stand();

			_collidingObjects[objectB] = side;
			this->fixPosition(side, objectB);
		}
	}
	else if (objectB == this)
	{
		eDirection side = this->getIntersectSide(objectA->getBoundingBox());
		CCLOG("intersect side: %s", tank::Converter::directionToString(side));

		if (side != eDirection::NONE && side == _direction)
		{
			this->stand();

			_collidingObjects[objectA] = side;
			this->fixPosition(side, objectA);
		}
	}

	return true;
}

void Tank::onContactSeparate(PhysicsContact & contact)
{
	auto objectA = (GameObject*)contact.getShapeA()->getBody()->getNode();
	auto objectB = (GameObject*)contact.getShapeB()->getBody()->getNode();
	
	this->removeCollidingObject(objectA);
	this->removeCollidingObject(objectB);
}

bool Tank::onPreSolve(PhysicsContact & contact, PhysicsContactPreSolve & presolve)
{
	auto objectA = (GameObject*)contact.getShapeA()->getBody()->getNode();
	auto objectB = (GameObject*)contact.getShapeB()->getBody()->getNode();
	
	if (objectA->getId() == objectB->getId() || 
		objectB->getId() == eObjectId::BULLET || objectA->getId() == eObjectId::BULLET)
		return true;
	
	if (objectA == this)
	{
		eDirection side = this->getIntersectSide(objectB->getBoundingBox());

		if (side != eDirection::NONE)
		{
			this->fixPosition(side, objectB);
		}
	}
	else if (objectB == this)
	{
		eDirection side = this->getIntersectSide(objectA->getBoundingBox());
		
		if (side != eDirection::NONE)
		{
			this->fixPosition(side, objectA);
		}
	}

	return true;
}

void Tank::updateWithStatus(eStatus status)
{
	switch (status)
	{
	case DIE:
	{
		if (this->getParent() == nullptr)
			break;

		auto explosion = Explosion::create(true);
		explosion->setPosition(this->getPosition());

		this->getParent()->addChild(explosion);
		this->runAction(RemoveSelf::create());

		break;
	}
	case NORMAL:
		break;
	case PROTECTED:
		break;
	case RUNNING:
		break;
	case STAND:
		break;
	default:
		break;
	}
}

void Tank::shoot()
{
	if (_bulletCounter >= this->getMaxBullet())
	{
		return;
	}

	Vec2 shootPosition = this->getPosition();
	float offset = 10.0f;

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

	auto bullet = Bullet::create(shootPosition, this->getDirection());
	bullet->setTag(GameObject::getNextId());
	bullet->setOwner(this);

	auto parent = this->getParent();
	if (parent != nullptr)
	{
		parent->addChild(bullet);
		_bulletCounter++;
	}
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

void Tank::fixPositionForTurn()
{
	if (_oldDirection == _direction)
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

	auto newPos = Vec2(TILE_WIDTH * (x), TILE_WIDTH * (y));
	auto delta = newPos - this->getPosition();

	if (delta.length() < TILE_WIDTH / 2)
	{
		this->setPosition(newPos);
	}

	_oldDirection = _direction;
}

bool Tank::isCollidingAtSide(eDirection side)
{
	for (auto it = _collidingObjects.begin(); it != _collidingObjects.end(); it++)
	{
		if (it->second == side)
		{
			return true;
		}
	}

	return false;
}

void Tank::removeCollidingObject(GameObject* object)
{
	_collidingObjects.erase(object);
}

void Tank::fixPosition(eDirection direction, GameObject* other)
{
	auto otherPosition = other->getPosition();
	auto delta = this->getPosition() - otherPosition;
	auto otherRect = other->getBoundingBox();
	auto myRect = this->getBoundingBox();

	if (abs(delta.x) >= otherRect.size.width / 2 + myRect.size.width / 2 || 
		abs(delta.y) >= otherRect.size.height / 2 + myRect.size.height / 2)
		return;

	switch (direction)
	{
	case NONE:
		break;
	case RIGHT:
		this->setPositionX(otherPosition.x - otherRect.size.width / 2 - myRect.size.width / 2);
		break;
	case DOWN:
		this->setPositionY(otherPosition.y + otherRect.size.height / 2 + myRect.size.height / 2);
		break;
	case LEFT:
		this->setPositionX(otherPosition.x + otherRect.size.width / 2 + myRect.size.width / 2);
		break;
	case UP:
		this->setPositionY(otherPosition.y - otherRect.size.height / 2 - myRect.size.height / 2);
		break;
	default:
		break;
	}
}

void Tank::stand()
{
	_velocity = 0;
	this->removeStatus(eStatus::RUNNING);
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
