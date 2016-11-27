#include "Wall.h"

Wall::Wall(eObjectId id) : GameObject(id)
{
}

Wall::~Wall()
{
}

Wall * Wall::createWithPosition(eObjectId type, const Vector2 &position)
{
	auto wall = new Wall(type);
	wall->setPosition(position);

	if (wall->init())
	{
		return wall;
	}

	delete wall;
	return nullptr;
}

bool Wall::init()
{
	_direction = eDirection::NONE;

	_boudingBox.width = 16;
	_boudingBox.height = 16;

	this->updateBoundingBox();

	_health = 2;

	return true;
}

void Wall::update(float dt)
{

}

void Wall::checkCollision(GameObject& object, float dt)
{

}

void Wall::gotHit(Damage* damage)
{
	_health -= damage->getValue();
	
	if (_health <= 0)
	{
		this->setStatus(eStatus::DIE);
		return;
	}

	//switch (damage->getDirection())
	//{
	//case LEFT:
	//	_direction = eDirection::RIGHT;
	//	break;
	//case UP:
	//	_direction = eDirection::DOWN;
	//	break;
	//case RIGHT:
	//	_direction = eDirection::LEFT;
	//	break;
	//case DOWN:
	//	_direction = eDirection::UP;
	//	break;
	//default:
	//	break;
	//}

	_direction = damage->getDirection();

	delete damage;

	this->updateBoundingBox();
}

void Wall::updateBoundingBox()
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
}
