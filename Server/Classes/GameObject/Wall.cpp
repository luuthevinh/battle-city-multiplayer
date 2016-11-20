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
	_boudingBox.position.x = this->getPosition().x - 16;
	_boudingBox.position.y = this->getPosition().y - 16;
	_boudingBox.width = 32;
	_boudingBox.height = 32;

	return true;
}

void Wall::update(float dt)
{

}

void Wall::checkCollision(GameObject& object, float dt)
{

}