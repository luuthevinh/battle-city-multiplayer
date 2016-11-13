#include "GameObject.h"
#include "SpriteManager.h"

GameObject::GameObject(eObjectId id) : 
	_id(id),
	_status(eStatus::NORMAL),
	_direction(eDirection::UP)
{
	_buffer = new Buffer(25);
	this->setName(SpriteManager::getInstance()->getObjectName(id));
}

GameObject::GameObject(Buffer& buffer)
{
	_buffer = new Buffer(25);
	this->deserialize(buffer);

	this->setName(SpriteManager::getInstance()->getObjectName(this->getId()));
}

GameObject::~GameObject()
{
	delete _buffer;
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

	// update with status
	this->updateWithStatus(_status);
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

void GameObject::setDirection(eDirection direction)
{
	if (_direction == direction || direction <= 0 || direction > 4)
		return;

	_direction = direction;
}

eDirection GameObject::getDirection()
{
	return _direction;
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

	data.setBeginRead(0);
}

void GameObject::updateWithStatus(eStatus status)
{
}