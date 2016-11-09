#include "GameObject.h"
#include "SpriteManager.h"

GameObject::GameObject(eObjectId id) : 
	_id(id),
	_status(eStatus::NORMAL)
{
	this->setName(SpriteManager::getInstance()->getObjectName(id));
}

GameObject::~GameObject()
{
}

void GameObject::addStatus(eStatus status)
{
	_status = (eStatus)(_status | status);
}

void GameObject::setStatus(eStatus status)
{
	_status = status;
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
