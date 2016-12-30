#include "Brick.h"
#include "..\Scene\PlayScene.h"

Brick::Brick() : Wall(eObjectId::BRICK_WALL)
{
}

Brick::~Brick()
{
}

bool Brick::init()
{
	if (!Wall::init())
	{
		return false;
	}

	this->updateBoundingBox();
	_health = 2;

	return true;
}

void Brick::gotHit(Damage* damage)
{
	_health -= damage->getValue();

	if (_health <= 0)
	{
		this->setStatus(eStatus::DIE);
		this->onChanged();

		this->updateMap();
		delete damage;
		return;
	}

	_direction = damage->getDirection();

	delete damage;

	this->updateBoundingBox();
}

void Brick::updateBoundingBox()
{
	switch (_direction)
	{
	case NONE:
		break;
	case LEFT:
		_boudingBox.width = _boudingBox.width / 2;
		_position.x -= _boudingBox.width / 2;
		break;
	case UP:
		_boudingBox.height = _boudingBox.height / 2;
		_position.y += _boudingBox.height / 2;
		break;
	case RIGHT:
		_boudingBox.width = _boudingBox.width / 2;
		_position.x += _boudingBox.width / 2;
		break;
	case DOWN:
		_boudingBox.height = _boudingBox.height / 2;
		_position.y -= _boudingBox.height / 2;
		break;
	default:
		break;
	}

	_boudingBox.position.x = this->getPosition().x - _boudingBox.width / 2;
	_boudingBox.position.y = this->getPosition().y - _boudingBox.height / 2;

	this->onChanged();
}

void Brick::updateMap()
{
	auto scene = (PlayScene*)_parentScene;
	if (scene == nullptr)
		return;

	scene->updateMap(this->getPosition(), eTiledObjectId::NONE_TILE);
}