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
	_boudingBox.position.x = this->getPosition().x - 8;
	_boudingBox.position.y = this->getPosition().y - 8;
	_boudingBox.width = 16;
	_boudingBox.height = 16;

	return true;
}

void Wall::update(float dt)
{

}

void Wall::checkCollision(GameObject& object, float dt)
{

}