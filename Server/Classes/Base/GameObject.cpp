﻿#include "GameObject.h"
#include "..\Shared\Buffer.h"

GameObject::GameObject(eObjectId id) :
	_id(id),
	_position(0, 0),
	_status(eStatus::STAND),
	_direction(eDirection::UP),
	_hasChanged(true)
{
	_buffer = new Buffer(25);
}

GameObject::GameObject(Buffer & buffer) :
	_hasChanged(true)
{
	_buffer = new Buffer(25);
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
