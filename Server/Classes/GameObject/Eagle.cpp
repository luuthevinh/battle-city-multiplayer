#include "Eagle.h"
#include "..\Base\SceneManager.h"
#include "..\Scene\OverScene.h"

Eagle::Eagle() : GameObject(eObjectId::EAGLE)
{
}

Eagle::~Eagle()
{
}

bool Eagle::init()
{
	_boudingBox.width = 32;
	_boudingBox.height = 32;

	this->updateBoundingBoxPosition();

	_health = 1;

	return true;
}

void Eagle::setPosition(const Vector2 & position)
{
	GameObject::setPosition(position);

	this->updateBoundingBoxPosition();
}

void Eagle::setStatus(eStatus status)
{
	GameObject::setStatus(status);
	this->updateWithStatus(status);
}

void Eagle::gotHit(Damage * damage)
{
	_health -= damage->getValue();

	if (_health <= 0)
	{
		this->setStatus(eStatus::DIE);
		this->onChanged();
	}

	delete damage;
}

void Eagle::updateBoundingBoxPosition()
{
	// update bounding box
	_boudingBox.position.x = this->getPosition().x - _boudingBox.width / 2;
	_boudingBox.position.y = this->getPosition().y - _boudingBox.height / 2;
}

void Eagle::updateWithStatus(eStatus status)
{
	switch (status)
	{
	case DIE:
	{
		SceneManager::getInstance()->getCurrentScene()->setOver(true);
		break;
	}
	default:
		break;
	}
}
