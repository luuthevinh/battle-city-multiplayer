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
