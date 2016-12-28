#include "Bullet.h"
#include "Base\SpriteManager.h"
#include "GameObject\Explosion.h"

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
	if (bullet->init())
	{
		bullet->autorelease();
		bullet->setPosition(position);
		bullet->setDirection(direction);

		return bullet;
	}

	CC_SAFE_DELETE(bullet);
	return nullptr;
}

Bullet * Bullet::createWithBuffer(Buffer & buffer)
{
	Bullet* bullet = new(std::nothrow) Bullet();

	if (bullet->init())
	{
		bullet->initWithBuffer(buffer);
		bullet->autorelease();
		return bullet;
	}

	CC_SAFE_DELETE(bullet);
	return nullptr;
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

	auto number = data.readFloat();

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
	
	if (_nextPosition != Vec2::ZERO && !_firstUpdated)
	{
		this->setPositionX(tank::lerp(_nextPosition.x, this->getPositionX(), _speed * dt));
		this->setPositionY(tank::lerp(_nextPosition.y, this->getPositionY(), _speed * dt));

	}
	else if(_firstUpdated)
	{
		this->predict(dt);
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
		this->runAction(RemoveSelf::create());
		break;
	}
	default:
		break;
	}
}

void Bullet::explode()
{
	this->setStatus(eStatus::DIE);

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
		this->explode();
		this->runAction(RemoveSelf::create());
	}
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

void Bullet::setOwner(GameObject* owner)
{
	_owner = owner;
}

GameObject* Bullet::getOwner()
{
	return _owner;
}

bool Bullet::onContactBegin(PhysicsContact & contact)
{
	auto objectA = (GameObject*)contact.getShapeA()->getBody()->getNode();
	auto objectB = (GameObject*)contact.getShapeB()->getBody()->getNode();

	if (objectA->getId() != eObjectId::BULLET && objectB->getId() != eObjectId::BULLET)
		return true;

	if (objectA != _owner && objectB != _owner)
	{
		if (objectA->getId() == eObjectId::BULLET && objectB->getId() == eObjectId::BULLET)
		{
			this->setStatus(eStatus::DIE);
		}
		else
		{
			this->explode();
		}
	}

	return true;
}
