#include "Wall.h"
#include "..\Scene\PlayScene.h"

#include "Brick.h"
#include "Steel.h"
#include "Grass.h"
#include "Ice.h"
#include "Water.h"

Wall::Wall(eObjectId id) : GameObject(id)
{
}

Wall::~Wall()
{
}

Wall * Wall::createWithPosition(eObjectId type, const Vector2 &position)
{
	Wall* wall = nullptr;

	switch (type)
	{
		case BRICK_WALL:
			wall = new Brick();
			break;
		case STEEL_WALL:
			wall = new Steel();
			break;
		case GRASS_WALL:
			wall = new Grass();
			break;
		case ICE_WALL:
			wall = new Ice();
			break;
		case WATER_WALL:
			wall = new Water();
			break;
		default:
			break;
	}

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

	return true;
}

void Wall::checkCollision(GameObject& object, float dt)
{

}

void Wall::updateBoundingBox()
{
	_boudingBox.position.x = this->getPosition().x - _boudingBox.width / 2;
	_boudingBox.position.y = this->getPosition().y - _boudingBox.height / 2;
}
