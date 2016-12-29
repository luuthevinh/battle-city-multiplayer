#include "ServerPlayScene.h"
#include "GameObject\Player.h"
#include "Base\ServerConnector.h"
#include "Base\GameObject.h"
#include "GameObject\AppearanceEffect.h"
#include "Base\ClientConverterFactory.h"
#include "Scene\OverScene.h"
#include "InfoLayer.h"

// shared
#include "..\Server\Classes\Shared\WorldSnapshot.h"
#include "..\Server\Classes\Shared\Converter.h"

#include "GameObject\Wall.h"
#include "GameObject\Bullet.h"
#include "GameObject\Eagle.h"


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

	GameObject::setLastUniqueId(0);

	this->setName("layer");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->initWithTMX();

	//auto player = Player::create(eObjectId::YELLOW_TANK);
	//player->setPosition(16, 144);
	//player->setName("player");
	//this->addChild(player);

	auto infoLayer = InfoLayer::create();
	infoLayer->setAnchorPoint(Vec2(0.0f, 0.0f));
	infoLayer->setPosition(416.0f, 0.0f);
	this->addChild(infoLayer);

	// update
	this->scheduleUpdate();

	return true;
}

void ServerPlayScene::update(float dt)
{
	if (ServerConnector::getInstance()->isRunning())
	{
		ServerConnector::getInstance()->update(dt);

		this->handleData();
	}
}

void ServerPlayScene::handleData()
{
	if (!ServerConnector::getInstance()->isConnected())
		return;

	auto data = ServerConnector::getInstance()->getFactory()->convertNext();

	if (data == nullptr)
		return;

	auto type = data->getType();

	switch (type)
	{
	case eDataType::OBJECT:
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(data);
		if (gameObject)
		{
			auto object = (GameObject*)this->getChildByTag(gameObject->getUniqueId());
			if (object == nullptr)
			{
				this->addChild(gameObject);
				return;
			}

			object->deserialize(*data->serialize());
		}
		break;
	}
	case eDataType::TANK:
	{
		Tank* tank = dynamic_cast<Tank*>(data);
		if (tank)
		{
			if (tank->getUniqueId() == ServerConnector::getInstance()->getUniqueId())
			{
				auto player = Player::createWithBuffer(*data->serialize());
				this->addChild(player);
				return;
			}

			auto object = (Tank*)this->getChildByTag(tank->getUniqueId());
			if (object == nullptr)
			{
				this->addChild(tank);
				return;
			}

			object->deserialize(*data->serialize());
		}
		break;
	}
	case eDataType::SNAPSHOT:
	{
		this->updateSnapshot((WorldSnapshot*)data);
		break;
	}
	case eDataType::INTEGER:
	{
		this->handleIntegerPacket((IntegerPacket*)data);
		break;
	}
	default:
		break;
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

	auto ids = snapshot->getDataObjects();

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

void ServerPlayScene::handleIntegerPacket(IntegerPacket * packet)
{
	switch (packet->integerType)
	{
	case IntegerPacket::GAME_OVER:
	{
		this->gameOver();
	}
	default:
		break;
	}
}

void ServerPlayScene::initWithTMX()
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

	auto objects = map->getObjectGroup("objects");
	auto flag = objects->getObject("flag");
	if (flag.size() > 0)
	{
		auto x = flag["x"].asFloat();
		auto y = flag["y"].asFloat();

		auto eagle = Eagle::create();
		eagle->setPosition(x + 16, y + 16);
		eagle->setTag(GameObject::getNextId());
		this->addChild(eagle);
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
	case BULLET:
	{
		auto bullet = (Bullet*)object;
		auto owner = this->getChildByTag(bullet->getOwnerTag());

		if(owner != nullptr)
		{
			bullet->setOwner((GameObject*)owner);
			this->addChild(bullet);
		}
		
		break;
	}
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

void ServerPlayScene::gameOver()
{
	auto sprite = Sprite::createWithSpriteFrameName("gameover.png");
	sprite->setPosition(14 * TILE_WIDTH, -50.0f);
	this->addChild(sprite);

	auto replaceScene = CallFunc::create([] {
		Director::getInstance()->replaceScene(OverScene::createScene());
	});

	sprite->runAction(Sequence::createWithTwoActions(
		MoveTo::create(2.0f, Vec2(sprite->getPositionX(), 16 * TILE_WIDTH)), 
		replaceScene));
}
