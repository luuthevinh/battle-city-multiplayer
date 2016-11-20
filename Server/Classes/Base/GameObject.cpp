#include "GameObject.h"
#include "..\Shared\Buffer.h"
#include "..\Game.h"
#include "AABB.h"

GameObject::GameObject(eObjectId id) :
	_id(id),
	_position(0, 0),
	_status(eStatus::STAND),
	_direction(eDirection::UP),
	_hasChanged(true),
	_boudingBox(0, 0, 0, 0),
	_collisionChecker(nullptr),
	_collisionBitmask(1),
	_categoryBitmask(1)
{
	_buffer = new Buffer(29);
}

GameObject::GameObject(Buffer & buffer) :
	_hasChanged(true),
	_boudingBox(0, 0, 0, 0),
	_collisionChecker(nullptr)
{
	_buffer = new Buffer(29);
	this->deserialize(buffer);
}

GameObject::~GameObject()
{
	delete _buffer;
}

bool GameObject::init()
{
	return true;
}

void GameObject::update(float dt)
{
}

void GameObject::setPosition(float x, float y)
{
	_position.x = x;
	_position.y = y;
}

void GameObject::setPosition(const Vector2 & position)
{
	_position.x = position.x;
	_position.y = position.y;
}

Vector2 GameObject::getPosition()
{
	return _position;
}

void GameObject::setDirection(eDirection direction)
{
	_direction = direction;
}

eDirection GameObject::getDirection()
{
	return _direction;
}

void GameObject::addStatus(eStatus status)
{
	_status = (eStatus)(_status | status);
}

void GameObject::removeStatus(eStatus status)
{
	_status = (eStatus)(_status & (~status));
}

void GameObject::setStatus(eStatus status)
{
	_status = status;
}

bool GameObject::hasStatus(eStatus status)
{
	if ((_status & status) == status)
		return true;

	return false;
}

eStatus GameObject::getStatus()
{
	return _status;
}

void GameObject::setId(eObjectId id)
{
	_id = id;
}

eObjectId GameObject::getId()
{
	return _id;
}

void GameObject::setTag(int tag)
{
	_tag = tag;
}

int GameObject::getTag()
{
	return _tag;
}

void GameObject::onChanged()
{
	_hasChanged = true;
}

void GameObject::setChanged(bool value)
{
	_hasChanged = value;
}

bool GameObject::hasChanged()
{
	return _hasChanged;
}

Buffer * GameObject::serialize()
{
	_buffer->setIndex(0);
	_buffer->setBeginRead(0);

	_buffer->writeInt(eDataType::OBJECT);
	_buffer->writeInt(this->getId());
	_buffer->writeInt(this->getTag());
	_buffer->writeInt(this->getStatus());
	_buffer->writeByte(this->getDirection());
	_buffer->writeFloat(this->getPosition().x);
	_buffer->writeFloat(this->getPosition().y);

	_buffer->writeFloat(Game::instance->getGameTime()->getTotalTime());

	return _buffer;
}

void GameObject::deserialize(Buffer & data)
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
	this->setPosition(x, y);

	float time = data.readFloat();

	data.setBeginRead(0);
}

Rect GameObject::getBoundingBox() const
{
	return _boudingBox;
}

Vector2 GameObject::getVelocity() const
{
	return Vector2(0, 0);
}

void GameObject::checkCollision(GameObject & other, float dt)
{
}

AABB * GameObject::getCollisionChecker()
{
	return _collisionChecker;
}

void GameObject::setCategoryBitmask(int category)
{
	_categoryBitmask = category;
}

int GameObject::getCategoryBitmask()
{
	return _categoryBitmask;
}

void GameObject::setCollisionBitmask(int collision)
{
	_collisionBitmask = collision;
}

int GameObject::getCollisionBitmask()
{
	return _collisionBitmask;
}

bool GameObject::canCollisionWith(int category)
{
	if ((_collisionBitmask & category) == category)
		return true;

	return false;
}
