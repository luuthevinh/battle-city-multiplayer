#include "Bullet.h"
#include "Base\SpriteManager.h"
#include "GameObject\Explosion.h"
#include "Base\ServerConnector.h"
#include "GameObject\Tank.h"

#include "..\Server\Classes\Shared\Utils.h"

Bullet::Bullet() : GameObject(eObjectId::BULLET),
	_direction(eDirection::UP)
{
}

Bullet::~Bullet()
{
}

Bullet * Bullet::create(const Vec2 & position, eDirection direction)
{
	Bullet* bullet = new(std::nothrow) Bullet();
	if (bullet && bullet->init())
	{
		bullet->autorelease();
		bullet->setPosition(position);
		bullet->setDirection(direction);

		return bullet;
	}

	CC_SAFE_DELETE(bullet);
	return nullptr;
}

Bullet * Bullet::createInfo(Buffer & buffer)
{
	Bullet* bullet = new(std::nothrow) Bullet();

	if (bullet)
	{
		bullet->initWithBuffer(buffer);

		buffer.setBeginRead(GameObject::INDEX_POSITION_X_BUFFER);
		float x = buffer.readFloat();
		float y = buffer.readFloat();
		bullet->setPosition(x, y);

		return bullet;
	}

	CC_SAFE_DELETE(bullet);
	return nullptr;
}

Bullet * Bullet::createGameObject(GameObject * info)
{
	Bullet* bullet = new(std::nothrow) Bullet();

	if (bullet && bullet->init())
	{
		auto data = info->serialize();
		bullet->deserialize(*data);

		data->setBeginRead(GameObject::INDEX_POSITION_X_BUFFER);
		float x = data->readFloat();
		float y = data->readFloat();
		bullet->setPosition(x, y);

		bullet->autorelease();
		return bullet;
	}

	CC_SAFE_DELETE(bullet);
	return nullptr;
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
	this->setTag(data.readInt());
	this->setStatus((eStatus)data.readInt());
	this->setDirection((eDirection)data.readByte());
	float x = data.readFloat();
	float y = data.readFloat();

	_ownerTag = data.readInt();

	data.setBeginRead(0);
}

bool Bullet::init()
{
	this->scheduleUpdate();

	_sprite = Sprite::createWithSpriteFrameName(SpriteManager::getInstance()->getObjectName(eObjectId::BULLET));
	this->addChild(_sprite);

	auto body = PhysicsBody::createBox(Size(BULLET_SIZE_WIDTH, BULLET_SIZE_WIDTH), PhysicsMaterial(0, 0, 0));
	this->setPhysicsBody(body);

	body->setCategoryBitmask(eObjectId::BULLET);
	body->setContactTestBitmask(eObjectId::ALL);
	body->getShapes().at(0)->setSensor(true);

	_level = eBulletLevel::SLOW_BULLET;
	_speed = this->getBulletSpeed();

	this->setZOrder(BULLET_Z_INDEX);
	this->setContentSize(Size(BULLET_SIZE_WIDTH, BULLET_SIZE_WIDTH));

	// listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Bullet::onContactBegin, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

void Bullet::update(float dt)
{
	GameObject::update(dt);
	
	if (_previousBuffers.size() > 1)
	{
		auto curTime = ServerConnector::getInstance()->getClientTime();
		auto interpolatedTime = curTime - 0.1f;

		int from, to;
		this->getFromToBufferIndex(from, to, interpolatedTime);

		this->interpolate(*_previousBuffers.at(from), *_previousBuffers.at(to), interpolatedTime);
	}

	// update sprite rotate
	switch (_direction)
	{
	case LEFT:
		this->setRotation(0.0f);
		break;
	case UP:
		this->setRotation(90.0f);
		break;
	case RIGHT:
		this->setRotation(180.0f);
		break;
	case DOWN:
		this->setRotation(-90.0f);
		break;
	default:
		break;
	}

	this->checkCollisionWithBouding();
}

void Bullet::predict(float dt)
{
	switch (_direction)
	{
	case LEFT:
		this->setPositionX(this->getPositionX() - _speed * dt);
		this->setRotation(0.0f);
		break;
	case UP:
		this->setPositionY(this->getPositionY() + _speed * dt);
		this->setRotation(90.0f);
		break;
	case RIGHT:
		this->setPositionX(this->getPositionX() + _speed * dt);
		this->setRotation(180.0f);
		break;
	case DOWN:
		this->setPositionY(this->getPositionY() - _speed * dt);
		this->setRotation(-90.0f);
		break;
	default:
		break;
	}
}

void Bullet::setDirection(eDirection direction)
{
	_direction = direction;
}

eDirection Bullet::getDirection()
{
	return _direction;
}

void Bullet::updateWithStatus(eStatus status)
{
	switch (status)
	{
	case DIE:
	{
		this->explode();
		this->runAction(RemoveSelf::create());
		this->removeRefInOwner();
		break;
	}
	default:
		break;
	}
}

void Bullet::explode()
{
	if (this->getParent() == nullptr)
		return;

	auto explosion = Explosion::create(false);
	explosion->setPosition(this->getPosition());

	this->getParent()->addChild(explosion);
}

void Bullet::checkCollisionWithBouding()
{
	auto position = this->getPosition();
	if (position.x < 0 || position.x > 26 * TILE_WIDTH || 
		position.y < 0 || position.y > 32 * TILE_WIDTH)
	{
		this->setStatus(eStatus::DIE);
	}
}

void Bullet::updateWithTankLevel(eTankLevel tanklevel)
{
	switch (tanklevel)
	{
	case DEFAULT_TANK:
		_level = eBulletLevel::NORMAL_BULLET;
		break;
	case BASIC_TANK:
		_level = eBulletLevel::SLOW_BULLET;
		break;
	case SECOND_TANK:
	case FAST_TANK:
		_level = eBulletLevel::NORMAL_BULLET;
		break;
	case THIRD_TANK:
	case POWER_TANK:
		_level = eBulletLevel::FAST_BULLET;
		break;
		break;
	case FOURTH_TANK:
	case ARMOR_TANK:
		_level = eBulletLevel::NORMAL_BULLET;
		break;
	default:
		break;
	}

	this->getBulletSpeed();
}

float Bullet::getBulletSpeed()
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
		break;
	}

	return _speed;
}

void Bullet::removeRefInOwner()
{
	auto parent = this->getParent();
	if (parent == nullptr)
		return;

	auto owner = (Tank*)parent->getChildByTag(_ownerTag);
	if (owner == nullptr || !owner->isRunning())
		return;

	//if (_owner == nullptr)
	//	return;

	owner->removeBullet(this->getUniqueId());
}

void Bullet::setOwner(Tank* owner)
{
	_owner = owner;
	if (_owner == nullptr)
		return;

	_ownerTag = owner->getTag();
}

Tank* Bullet::getOwner()
{
	return _owner;
}

bool Bullet::onContactBegin(PhysicsContact & contact)
{
	auto objectA = (GameObject*)contact.getShapeA()->getBody()->getNode();
	auto objectB = (GameObject*)contact.getShapeB()->getBody()->getNode();

	if (objectA->getId() != eObjectId::BULLET && objectB->getId() != eObjectId::BULLET)
		return true;

	if (objectA->getId() == eObjectId::BULLET)
	{
		this->contactWithOtherObject(objectB);
	}
	else
	{
		this->contactWithOtherObject(objectA);
	}

	return true;
}

void Bullet::contactWithOtherObject(GameObject * object)
{
	if (object->getTag() == _ownerTag)
		return;

	auto owner = (Tank*)this->getParent()->getChildByTag(_ownerTag);
	if (owner == nullptr || !owner->isRunning())
		return;

	if (owner->getId() == object->getId())
	{
		return;
	}

	if (object->getId() == eObjectId::BULLET)
	{
		this->runAction(RemoveSelf::create());
		this->removeRefInOwner();
	}
	else if (object->getId() == eObjectId::BRICK_WALL || object->getId() == eObjectId::STEEL_WALL)
	{
		this->setStatus(eStatus::DIE);
		this->removeRefInOwner();
	}
}

void Bullet::reconcile(Buffer &data)
{
	if (_previousBuffers.size() == 0)
	{
		this->deserialize(data);
	}

	this->addLastBuffer(data);
}

int Bullet::getOwnerTag()
{
	return _ownerTag;
}
