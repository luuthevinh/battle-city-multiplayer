#include "ServerPlayScene.h"
#include "GameObject\Player.h"
#include "Base\ServerConnector.h"
#include "Base\GameObject.h"
#include "GameObject\AppearanceEffect.h"

// shared
#include "..\Server\Classes\Shared\WorldSnapshot.h"
#include "..\Server\Classes\Shared\Converter.h"
#include "GameObject\Wall.h"


ServerPlayScene::ServerPlayScene()
{
}

ServerPlayScene::~ServerPlayScene()
{
}

Scene * ServerPlayScene::createSceneWithLayer(Layer * layer)
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2::ZERO);

	scene->addChild(layer);

	return scene;
}

Scene * ServerPlayScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2::ZERO);

	auto layer = ServerPlayScene::create();
	scene->addChild(layer);

	return scene;
}

bool ServerPlayScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setName("layer");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->initWithTMX();

	//auto player = Player::create(eObjectId::YELLOW_TANK);
	//player->setPosition(16, 144);
	//player->setName("player");
	//this->addChild(player);

	// update
	this->scheduleUpdate();


	return true;
}

void ServerPlayScene::update(float dt)
{
	if (ServerConnector::getInstance()->isRunning())
	{
		ServerConnector::getInstance()->update(dt);
		ServerConnector::getInstance()->handleData(this);
	}
}

void ServerPlayScene::updateSnapshot(WorldSnapshot * snapshot)
{
	WorldSnapshot::id = snapshot->getUniqueId();

	// cập nhật luôn vầy nghĩa là ko có lag trên đường truyền
	ServerConnector::getInstance()->setTime(snapshot->getServerTime());

	if (GameObject::getLastUniqueId() != snapshot->getLastUniqueId())
	{
		GameObject::setLastUniqueId(snapshot->getLastUniqueId());
	}

	auto ids  = snapshot->getDataObjects();

	for (auto it = ids.begin(); it != ids.end(); it++)
	{
		auto node = (GameObject*)this->getChildByTag(it->first);

		if (node != nullptr)
		{
			//node->deserialize(*(it->second));
			node->reconcile(*(it->second));
		}
		else if (it->first == ServerConnector::getInstance()->getUniqueId())
		{
			auto player = Player::createWithBuffer(*(it->second));
			this->addChild(player);
		}
		else
		{
			auto gameObject = GameObject::createWithBuffer(*(it->second));
			if (gameObject)
			{
				this->addObject(gameObject);
			}
		}
	}
}

void ServerPlayScene::initWithTMX()
{
	auto map = TMXTiledMap::create("map/map_01.tmx");
	// addChild(map);

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

void ServerPlayScene::addWall(const Vec2 & position, eObjectId id)
{
	auto wall = Wall::createWithType(id);
	wall->setPosition(position + Vec2(8, 8));
	wall->setTag(GameObject::getNextId());

	this->addChild(wall);
}

void ServerPlayScene::addObject(GameObject * object)
{
	auto type = object->getId();
	switch (type)
	{
	case YELLOW_TANK:
	case GREEN_TANK:
	case WHITE_TANK:
		this->addTank(object);
		break;
	default:
		this->addChild(object);
		break;
	}
}

void ServerPlayScene::addTank(GameObject * tank)
{
	auto result = this->findTankPendingByUniqueId(tank->getUniqueId());
	if (result != nullptr)
		return;

	auto func = CallFunc::create([&] {	
		auto t = _tankPending.front();
		
		this->addChild(t);
		t->release();

		_tankPending.pop_front();
	});

	auto effect = AppearanceEffect::create();
	effect->setPosition(tank->getPosition());
	this->addChild(effect);

	_tankPending.push_back(tank);
	tank->retain();

	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), func));
}

GameObject * ServerPlayScene::findTankPendingByUniqueId(int id)
{
	for (auto i = _tankPending.begin(); i < _tankPending.end(); i++)
	{
		if ((*i)->getUniqueId() == id)
			return (*i);
	}

	return nullptr;
}

void ServerPlayScene::removeTankPendingByUniqueId(int id)
{
	for (auto i = _tankPending.begin(); i < _tankPending.end(); i++)
	{
		if ((*i)->getUniqueId() == id)
		{
			_tankPending.erase(i);
			return;
		}
	}
}
