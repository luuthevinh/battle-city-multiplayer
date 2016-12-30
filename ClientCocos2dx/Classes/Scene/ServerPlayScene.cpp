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

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

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

	_infoLayer = InfoLayer::create();
	_infoLayer->setAnchorPoint(Vec2(0.0f, 0.0f));
	_infoLayer->setPosition(416.0f, 0.0f);
	_infoLayer->addWave(1);
	this->addChild(_infoLayer);

	// update
	this->scheduleUpdate();

	_factory = ServerConnector::getInstance()->getFactory();

	// audio
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("levelstarting.wav", false);

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

	auto data = _factory->convertNext();

	if (data == nullptr)
		return;

	auto type = data->getType();

	switch (type)
	{
		case eDataType::OBJECT:
		{
			GameObject* gameObject = (GameObject*)(data);
			if (gameObject)
			{
				auto object = (GameObject*)this->getChildByTag(data->getUniqueId());
				if (object == nullptr)
				{
					auto object = GameObject::createGameObject(gameObject);
					this->addChild(object);
					CC_SAFE_RELEASE(gameObject);

					return;
				}

				object->deserialize(*data->serialize());
			}

			CC_SAFE_RELEASE(gameObject);

			break;
		}
		case eDataType::TANK:
		{
			Tank* tank = (Tank*)(data);
			if (tank)
			{
				if (tank->getUniqueId() == ServerConnector::getInstance()->getUniqueId())
				{
					auto player = Player::createGameObject(tank);
					this->addChild(player);
					CC_SAFE_RELEASE(tank);
					return;
				}

				auto object = (Tank*)this->getChildByTag(tank->getUniqueId());
				if (object == nullptr)
				{
					auto t = Tank::createGameObject(tank);
					this->addChild(t);
					CC_SAFE_RELEASE(tank);
					return;
				}

				object->deserialize(*data->serialize());
			}

			CC_SAFE_RELEASE(tank);

			break;
		}
		case eDataType::SNAPSHOT:
		{
			this->updateSnapshot((WorldSnapshot*)data);
			CC_SAFE_DELETE(data);
			break;
		}
		case eDataType::INTEGER:
		{
			this->handleIntegerPacket((IntegerPacket*)data);
			CC_SAFE_DELETE(data);
			break;
		}
		default:
			CC_SAFE_DELETE(data);
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
			auto info = Player::createInfo(*(it->second));
			auto player = Player::createGameObject(info);
			this->addChild(player);
			info->release();
		}
		else
		{
			auto gameObject = GameObject::createInfo(*(it->second));
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
		break;
	}
	case IntegerPacket::PLAYER_REVIVE:
	{
		this->revivePlayer(packet->value);
		break;
	}
	case IntegerPacket::WAVE:
	{
		_infoLayer->addWave(packet->value - _infoLayer->getCurrentWave());
		break;
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
		{
			this->addTank(object);
			break;
		}
		case BULLET:
		{
			auto bullet = (Bullet*)object;
			auto owner = this->getChildByTag(bullet->getOwnerTag());
			if (owner == nullptr)
				return;

			if (owner->getTag() == ServerConnector::getInstance()->getUniqueId())
			{
				if (auto tank = dynamic_cast<Tank*>(owner))
				{
					tank->updateBulletIdFromServer(*bullet);
				}
			}
			else
			{
				auto b = Bullet::createGameObject(bullet);
				b->setOwner((Tank*)owner);
				this->addChild(b);
			}
			

			object->release();
			break;
		}
		default:
		{
			auto obj = GameObject::createGameObject(object);
			this->addChild(obj);

			object->release();
			break;
		}
	}
}

void ServerPlayScene::addTank(GameObject * tank)
{
	auto result = this->findTankPendingByUniqueId(tank->getUniqueId());
	if (result != nullptr)
	{
		tank->release();
		return;
	}

	auto func = CallFunc::create([&] {	
		auto t = _tankPending.front();
		
		auto newtank = Tank::createGameObject(t);
		this->addChild(newtank);

		_tankPending.front()->release();
		_tankPending.pop_front();
	});

	auto effect = AppearanceEffect::create();
	effect->setPosition(tank->getPosition());
	this->addChild(effect);

	_tankPending.push_back(tank);

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
	SimpleAudioEngine::getInstance()->playEffect("gameover.wav", false);

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

void ServerPlayScene::revivePlayer(int tag)
{
	auto player = (Player*)this->getChildByTag(tag);
	if (player)
	{
		auto effect = AppearanceEffect::create();
		effect->setPosition(player->getPosition());
		this->addChild(effect);

		((Player*)player)->revive();

		if (player->getId() == eObjectId::YELLOW_TANK)
		{
			_infoLayer->setPlayerLife(1, player->getLife() - 1);
		}
		else
		{
			_infoLayer->setPlayerLife(2, player->getLife() - 1);
		}
		
	}
}

void ServerPlayScene::addChildAndAutoRelease(GameObject * object)
{
	object->autorelease();
	this->addChild(object);
}
