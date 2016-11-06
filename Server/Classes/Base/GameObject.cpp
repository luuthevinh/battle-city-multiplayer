#include "GameObject.h"

GameObject::GameObject(eObjectId id) :
	_id(id),
	_position(0, 0),
	_status(eStatus::STAND),
	_hasChanged(false)
{
}

GameObject::~GameObject()
{
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

void GameObject::addStatus(eStatus status)
{
	_status = (eStatus)(_status | status);
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

Packet GameObject::getPacket()
{
	return _packet;
}
