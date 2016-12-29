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
	_whiteTankCounter = 0;
	_yellowTankCounter = 0;
	_greenTankCounter = 0;
	_maxBots = 10;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//_playLayer = ServerPlayScene::create();
	//_playLayer->retain();

	this->scheduleUpdate();

	auto background = Sprite::createWithSpriteFrameName("waiting_scene_bg.png");
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(background);

	_textField = ui::TextField::create("Enter your name...", "/fonts/pixel.ttf", 21.0f);
	_textField->setAnchorPoint(Vec2(0.0f, 1.0f));
	_textField->setTextAreaSize(Size(325, 36));
	_textField->setPosition(Vec2(45, 512 - 58));
	_textField->addEventListener(CC_CALLBACK_2(WaitingScene::nameTextFieldEvent, this));
	this->addChild(_textField);

	_pointerInput = Sprite::createWithSpriteFrameName("arrowBtn.png");
	_pointerInput->setPosition(_textField->getPositionX() - 16, _textField->getPositionY() - 16);
	_pointerInput->setVisible(false);
	_pointerInput->setScaleX(-1.0f);
	this->addChild(_pointerInput);

	_addrTextField = ui::TextField::create(SERVER_ADD, "/fonts/pixel.ttf", 21.0f);
	_addrTextField->setAnchorPoint(Vec2(0.0f, 1.0f));
	_addrTextField->setTextAreaSize(Size(325, 36));
	_addrTextField->setPosition(Vec2(45, 512 - 120));
	_addrTextField->addEventListener(CC_CALLBACK_2(WaitingScene::nameTextFieldEvent, this));
	this->addChild(_addrTextField);

	// play btn
	auto joinBtn = ui::Button::create("joinBtn.png", "joinBtn_selected.png");
	joinBtn->setPosition(Vec2(429, 512 - 70));
	joinBtn->addTouchEventListener(CC_CALLBACK_2(WaitingScene::joinBtnTouchEvent, this));
	this->addChild(joinBtn);

	// play bt
	auto playBtn = ui::Button::create("playBtn.png", "playBtn_selected.png");
	playBtn->setPosition(Vec2(429, 512 - 141));
	playBtn->addTouchEventListener(CC_CALLBACK_2(WaitingScene::playBtnTouchEvent, this));
	this->addChild(playBtn);

	// back btn
	auto backBtn = ui::Button::create("backBtn.png", "backBtn_selected.png");
	backBtn->setPosition(Vec2(429, 512 - 182));
	backBtn->addTouchEventListener(CC_CALLBACK_2(WaitingScene::backBtnTouchEvent, this));
	this->addChild(backBtn);

	// number
	_yellowTankNumber = Label::createWithTTF("0", "/fonts/pixel.ttf", 21.0f);
	_yellowTankNumber->setPosition(92, 512 - 265);
	this->addChild(_yellowTankNumber);

	_greenTankNumber = Label::createWithTTF("0", "/fonts/pixel.ttf", 21.0f);
	_greenTankNumber->setPosition(256, 512 - 265);
	this->addChild(_greenTankNumber);

	_whiteTankNumber = Label::createWithTTF("0", "/fonts/pixel.ttf", 21.0f);
	_whiteTankNumber->setPosition(418, 512 - 265);
	this->addChild(_whiteTankNumber);

	// bot btn
	auto decreaseBtn = ui::Button::create("arrowBtn.png", "arrowBtn_selected.png");
	decreaseBtn->setPosition(Vec2(390, _whiteTankNumber->getPositionY()));
	decreaseBtn->addTouchEventListener(CC_CALLBACK_2(WaitingScene::decreaseBtnTouchEvent, this));
	this->addChild(decreaseBtn);

	auto increaseBtn = ui::Button::create("arrowBtn.png", "arrowBtn_selected.png");
	increaseBtn->setScaleX(-1.0f);
	increaseBtn->setPosition(Vec2(448, _whiteTankNumber->getPositionY()));
	increaseBtn->addTouchEventListener(CC_CALLBACK_2(WaitingScene::increaseBtnTouchEvent, this));
	this->addChild(increaseBtn);

	// tank btn
	auto yellowTankBtn = ui::Button::create("tank_01.png", "tank_01.png");
	yellowTankBtn->setPosition(Vec2(_yellowTankNumber->getPositionX(), 512 - 315));
	yellowTankBtn->addTouchEventListener(CC_CALLBACK_2(WaitingScene::yellowBtnTouchEvent, this));
	this->addChild(yellowTankBtn);

	auto greenTankBtn = ui::Button::create("tank_green_01.png", "tank_green_01.png");
	greenTankBtn->setPosition(Vec2(_greenTankNumber->getPositionX(), yellowTankBtn->getPositionY()));
	greenTankBtn->addTouchEventListener(CC_CALLBACK_2(WaitingScene::greenBtnTouchEvent, this));
	this->addChild(greenTankBtn);

	auto whiteTankBtn = ui::Button::create("tank_white_01.png", "tank_white_01.png");
	whiteTankBtn->setPosition(Vec2(_whiteTankNumber->getPositionX(), yellowTankBtn->getPositionY()));
	this->addChild(whiteTankBtn);

	_pointerPlayerSelect = Sprite::createWithSpriteFrameName("arrowBtn.png");
	_pointerPlayerSelect->setPosition(yellowTankBtn->getPositionX(), yellowTankBtn->getPositionY() - 20);
	_pointerPlayerSelect->setRotation(90.0f);
	this->addChild(_pointerPlayerSelect);

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
}

void WaitingScene::onKeyPressed(EventKeyboard::KeyCode keycode, Event * e)
{
}

void WaitingScene::nameTextFieldEvent(Ref * pSender, cocos2d::ui::TextField::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
	{
		_pointerInput->setVisible(true);
		_pointerInput->setPositionY(((ui::Button*)pSender)->getPositionY() - 16);
		break;
	}
	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
	{
		break;
	}
	case cocos2d::ui::TextField::EventType::INSERT_TEXT:
		break;
	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
		break;
	default:
		break;
	}
}

void WaitingScene::playBtnTouchEvent(Ref * sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		this->gotoPlayScene();
	}
}

void WaitingScene::backBtnTouchEvent(Ref * sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		
	}
}

void WaitingScene::joinBtnTouchEvent(Ref * sender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	{
		auto connector = ServerConnector::getInstance();
		if (!connector->isConnected())
		{
			auto address = _addrTextField->getString();
			if (address.compare("") == 0)
			{
				connector->init(PORT, SERVER_ADD);
			}
			else
			{

				connector->init(PORT, address.c_str());
			}
			connector->connectServer();
		}
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
	
}

void WaitingScene::decreaseBtnTouchEvent(Ref * sender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	{
		//_whiteTankCounter--;
		//if (_whiteTankCounter < 0)
		//{
		//	_whiteTankCounter = _maxBots;
		//}

		//this->updateWhiteNumberText();
		this->updateNumberOfBots(-1);

		break;
	}
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}

}

void WaitingScene::increaseBtnTouchEvent(Ref * sender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	{
		//_whiteTankCounter++;
		//if (_whiteTankCounter > _maxBots)
		//{
		//	_whiteTankCounter = 0;
		//}
		//this->updateWhiteNumberText();
		this->updateNumberOfBots(1);

		break;
	}
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void WaitingScene::yellowBtnTouchEvent(Ref * sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		_pointerPlayerSelect->setPositionX(((ui::Button*)sender)->getPositionX());
		this->updateAndSendPlayerId(eObjectId::YELLOW_TANK);
	}
}

void WaitingScene::greenBtnTouchEvent(Ref * sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		_pointerPlayerSelect->setPositionX(((ui::Button*)sender)->getPositionX());
		this->updateAndSendPlayerId(eObjectId::GREEN_TANK);
	}
}

void WaitingScene::handleData()
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
		//Player* player = dynamic_cast<Player*>(data);
		//if (player)
		//{
		//	if (player->getUniqueId() == ServerConnector::getInstance()->getUniqueId())
		//	{
		//		_currentPlayer->setPosition(player->getPosition());
		//	}
		//}

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
				ServerConnector::getInstance()->setUniqueId(packet->getUniqueId());
			} 
			else if (packet->beginTime == -1.0f)
			{
				_isReady = true;

				return;
			}
		}
		break;
	}
	case eDataType::INTEGER:
	{
		auto packet = dynamic_cast<IntegerPacket*>(data);
		if (packet && packet->integerType == IntegerPacket::Type::SET_HOST)
		{
			ServerConnector::getInstance()->setHost(packet->value);
		}
		break;
	}
	case eDataType::ROOM_INFO:
	{
		RoomInfo* packet = (RoomInfo*)data;
		if (packet)
		{
			_greenTankCounter = 0;
			_yellowTankCounter = 0;
			_whiteTankCounter = 0;

			for (auto it = packet->playerCounters.begin(); it != packet->playerCounters.end(); it++)
			{
				switch (it->first)
				{
				case eObjectId::GREEN_TANK:
				{
					_greenTankCounter = it->second;
					
					break;
				}
				case eObjectId::YELLOW_TANK:
				{
					_yellowTankCounter = it->second;
					break;
				}
				case eObjectId::WHITE_TANK:
				{
					_whiteTankCounter = it->second;
					break;
				}
				default:
					break;
				}
			}

			this->updateGreenNumberText();
			this->updateYellowNumberText();
			this->updateWhiteNumberText();
		}
		break;
	}
	default:
		break;
	}
}

void WaitingScene::gotoPlayScene()
{
	auto playScene = ServerPlayScene::createScene();
	Director::getInstance()->replaceScene(playScene);

	auto play = new IntegerPacket();
	play->integerType = IntegerPacket::BEGIN_PLAY;
	play->setUniqueId(ServerConnector::getInstance()->getUniqueId());

	ServerConnector::getInstance()->send(play);
	delete play;
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

	// _playLayer->addChild(player);

	// gửi info object lại
	ServerConnector::getInstance()->send(player);
}

void WaitingScene::updateYellowNumberText()
{
	auto str = StringUtils::format("%d", _yellowTankCounter);
	_yellowTankNumber->setString(str);
}

void WaitingScene::updateGreenNumberText()
{
	auto str = StringUtils::format("%d", _greenTankCounter);
	_greenTankNumber->setString(str);
}

void WaitingScene::updateWhiteNumberText()
{
	auto str = StringUtils::format("%d", _whiteTankCounter);
	_whiteTankNumber->setString(str);
}

void WaitingScene::updateAndSendPlayerId(eObjectId id)
{
	_playerSelected = id;
	auto integer = new IntegerPacket();
	integer->integerType = IntegerPacket::PLAYER_CHARACTER_SELECTION;
	integer->value = id;
	integer->setUniqueId(ServerConnector::getInstance()->getUniqueId());

	ServerConnector::getInstance()->send(integer);

	delete integer;
}

void WaitingScene::updateNumberOfBots(int value)
{
	if (!ServerConnector::getInstance()->isHost())
		return;

	auto integer = new IntegerPacket();
	integer->integerType = IntegerPacket::SET_BOT;
	integer->value = value;
	integer->setUniqueId(ServerConnector::getInstance()->getUniqueId());

	ServerConnector::getInstance()->send(integer);

	delete integer;
}
