#include "SinglePlayScene.h"
#include "GameObject\Wall.h"
#include "GameObject\Player.h"

// shared
#include "..\Server\Classes\Shared\Converter.h"

Scene * SinglePlayScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2::ZERO);

	auto layer = SinglePlayScene::create();
	scene->addChild(layer);

	return scene;
}

SinglePlayScene::SinglePlayScene()
{
}

SinglePlayScene::~SinglePlayScene()
{
}

bool SinglePlayScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	this->initWithTMX();

	// player
	auto player = Player::create(eObjectId::YELLOW_TANK);
	player->setPosition(144, 16);
	player->setName("player");
	this->addChild(player);

	return true;
}

void SinglePlayScene::initWithTMX()
{
	auto map = TMXTiledMap::create("map/map_01.tmx");

	auto layer = map->getLayer("wall");
	auto size = layer->getLayerSize();

	for (size_t y = 0; y < size.height; y++)
	{
		for (size_t x = 0; x < size.width; x++)
		{
			auto tileId = layer->getTileGIDAt(Vec2(x, y));
			auto tile = layer->getTileAt(Vec2(x, y));

			switch (tileId)
			{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			{
				this->addWall(tile->getPosition(), tank::Converter::tiledIdToObjectId(tileId));
				break;
			}
			default:
				break;
			}
		}
	}
}

void SinglePlayScene::addWall(const Vec2 & position, eObjectId id)
{
	auto wall = Wall::createWithType(id);
	wall->setPosition(position + Vec2(8, 8));

	this->addChild(wall);
}
