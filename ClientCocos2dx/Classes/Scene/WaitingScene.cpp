#include "WaitingScene.h"
#include "Base\ServerConnector.h"
#include "Base\ClientConverterFactory.h"
#include "Base\GameObject.h"
#include "GameObject\Player.h"
#include "ServerPlayScene.h"
#include "GameObject\TankCursor.h"

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

	_isReady = false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto connector = ServerConnector::getInstance();
	connector->init(PORT, SERVER_ADD);

	connector->connectServer();

	_playLayer = ServerPlayScene::create();
	_playLayer->retain();

	this->scheduleUpdate();

	auto message = Label::createWithTTF("LOADING...", "/fonts/pixel.ttf", 15.0f);
	message->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 128);
	message->setName("message");
	this->addChild(message);

	auto teamYellow = Label::createWithTTF("YELLOW TANK", "/fonts/pixel.ttf", 24.0f);
	teamYellow->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(teamYellow);

	auto teamGreen = Label::createWithTTF("GREEN TANK", "/fonts/pixel.ttf", 24.0f);
	teamGreen->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 32);
	this->addChild(teamGreen);

	_cursor = TankCursor::create();
	_cursor->addPosition(0, Vec2(teamYellow->getPosition().x - 128, teamYellow->getPosition().y));
	_cursor->addPosition(1, Vec2(teamGreen->getPosition().x - 128, teamGreen->getPosition().y));
	this->addChild(_cursor);

	// listener
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(WaitingScene::onKeyPressed, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

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

	// demo
	if (_isReady && message->getString().compare("READY") != 0)
	{
		message->setString("READY! PRESS ENTER TO PLAY.");
	}
}

void WaitingScene::onKeyPressed(EventKeyboard::KeyCode keycode, Event * e)
{
	if (keycode == EventKeyboard::KeyCode::KEY_ENTER)
	{
		if (!_isReady)
			return;

		auto index = _cursor->getCurrentIndex();
		switch (index)
		{
		case 0:
		{
			this->createPlayer(eObjectId::YELLOW_TANK);
			break;
		}
		case 1:
		{
			this->createPlayer(eObjectId::GREEN_TANK);
			break;
		}
		default:
			break;
		}

		this->gotoPlayScene();
	}
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
		if (gameObject->getTag() == ServerConnector::getInstance()->getUniqueId())
		{
			return;
		}
		if (gameObject)
		{
			auto object = _playLayer->getChildByTag(gameObject->getTag());
			if (object == nullptr)
			{
				_playLayer->addChild(gameObject);
				return;
			}

			//((GameObject*)object)->reconcile(*data->serialize());
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
				ServerConnector::getInstance()->setUniqueId(packet->uniqueId);
			} 
			else if (packet->beginTime == -1.0f)
			{
				_isReady = true;

				return;
			}
		}
	}
	default:
		break;
	}
}

void WaitingScene::gotoPlayScene()
{
	auto playScene = ServerPlayScene::createSceneWithLayer(_playLayer);
	Director::getInstance()->replaceScene(playScene);

	_playLayer->release();
}

void WaitingScene::createPlayer(eObjectId id)
{
	Vec2 position = Vec2(0, 0);

	// tạm để này để demo
	switch (id)
	{
	case YELLOW_TANK:
		position = Vec2(160, 16);
		break;
	case GREEN_TANK:
		position = Vec2(256, 16);
		break;
	case WHITE_TANK:
		break;
	}

	auto player = Player::create(id);
	player->setPosition(position);
	player->setTag(ServerConnector::getInstance()->getUniqueId());
	player->setName("player");

	_playLayer->addChild(player);

	// gửi info object lại
	ServerConnector::getInstance()->send(player);
}
