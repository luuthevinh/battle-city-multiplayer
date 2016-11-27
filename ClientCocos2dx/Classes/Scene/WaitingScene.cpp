#include "WaitingScene.h"
#include "Base\ServerConnector.h"
#include "Base\ClientConverterFactory.h"
#include "Base\GameObject.h"
#include "GameObject\Player.h"
#include "ServerPlayScene.h"

// shared
#include "..\Server\Classes\Shared\DataPacket.h"

WaitingScene::WaitingScene()
{
}

WaitingScene::~WaitingScene()
{
}

Scene * WaitingScene::createScene()
{
	auto scene = Scene::create();

	auto layer = WaitingScene::create();
	scene->addChild(layer);

	return scene;
}

bool WaitingScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto connector = ServerConnector::getInstance();
	connector->init(PORT, SERVER_ADD);

	connector->connectServer();

	_playLayer = ServerPlayScene::create();
	_playLayer->retain();

	this->scheduleUpdate();

	auto message = Label::createWithTTF("LOADING...", "/fonts/pixel.ttf", 15.0f);
	message->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	message->setName("message");
	this->addChild(message);

	return true;
}

void WaitingScene::update(float dt)
{
	ServerConnector::getInstance()->update(dt);

	this->handleData();

	//
	auto message = (Label*)this->getChildByName("message");
	auto messStr = String::createWithFormat("LOADING FROM SERVER: %d", _playLayer->getChildren().size());
	message->setString(messStr->getCString());
}

void WaitingScene::handleData()
{
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
			auto object = _playLayer->getChildByTag(gameObject->getTag());
			if (object == nullptr)
			{
				_playLayer->addChild(gameObject);
				return;
			}

			((GameObject*)object)->deserialize(*data->serialize());
		}
		break;
	}
	case eDataType::REPLY_ID:
	{
		ReplyPacket* packet = dynamic_cast<ReplyPacket*>(data);
		if (packet)
		{
			if (packet->beginTime > 0)
			{
				ServerConnector::getInstance()->setTime(packet->beginTime);
				ServerConnector::getInstance()->setRun(true);

				auto object = _playLayer->getChildByName("player");
				if (object == nullptr)
					return;

				((Player*)object)->setTag(packet->uniqueId);
			} 
			else if (packet->beginTime == -1.0f)
			{
				auto playScene = ServerPlayScene::createSceneWithLayer(_playLayer);
				Director::getInstance()->replaceScene(playScene);

				_playLayer->release();

				return;
			}
		}
	}
	default:
		break;
	}
}
