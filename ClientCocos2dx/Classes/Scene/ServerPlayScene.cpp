#include "ServerPlayScene.h"
#include "GameObject\Player.h"
#include "Base\ServerConnector.h"
#include "Base\GameObject.h"

// shared
#include "..\Server\Classes\Shared\WorldSnapshot.h"


ServerPlayScene::ServerPlayScene()
{
}

ServerPlayScene::~ServerPlayScene()
{
}

Scene * ServerPlayScene::createSceneWithLayer(Layer * layer)
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2::ZERO);

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

	// update
	this->scheduleUpdate();

	return true;
}

void ServerPlayScene::update(float dt)
{
	ServerConnector::getInstance()->update(dt);

	ServerConnector::getInstance()->handleData(this);
}

void ServerPlayScene::updateSnapshot(WorldSnapshot * snapshot)
{
	WorldSnapshot::id = snapshot->getUniqueId();

	// cập nhật luôn vầy nghĩa là ko có lag trên đường truyền
	ServerConnector::getInstance()->setTime(snapshot->getServerTime());

	auto ids  = snapshot->getDataObjects();

	for (auto it = ids.begin(); it != ids.end(); it++)
	{
		auto node = (GameObject*)this->getChildByTag(it->first);
		if (node != nullptr)
		{
			//node->deserialize(*(it->second));
			node->reconcile(*(it->second));
		}
		else
		{
			auto gameObject = GameObject::createWithBuffer(*(it->second));
			if (gameObject)
			{
				this->addChild(gameObject);
			}
		}
	}
}
