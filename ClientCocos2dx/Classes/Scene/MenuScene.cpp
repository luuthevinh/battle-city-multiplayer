#include "MenuScene.h"
#include "WaitingScene.h"
#include "ServerPlayScene.h"
#include "Base\SpriteManager.h"
#include "SinglePlayScene.h"
#include "SimpleAudioEngine.h"

#include "..\Server\Classes\Shared\DataPacket.h"
#include "Base\ServerConnector.h"

using namespace CocosDenshion;

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

Scene * MenuScene::createScene()
{
	auto scene = Scene::create();

	auto layer = MenuScene::create();
	scene->addChild(layer);

	return scene;
}

bool MenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	/////////////////////////////
	SpriteManager::getInstance()->init();

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto sprite = Sprite::create("title.png");
	sprite->setPosition(visibleSize.width / 2, 2 * visibleSize.height / 3);
	this->addChild(sprite);

	auto single = Label::createWithTTF("SINGLE PLAYER", "/fonts/pixel.ttf", 18.0f);
	//single->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	//this->addChild(single);

	auto multi = Label::createWithTTF("MULIPLAYER", "/fonts/pixel.ttf", 18.0f);
	//multi->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	//this->addChild(joinServer);

	auto singleItem = MenuItemLabel::create(single, CC_CALLBACK_1(MenuScene::onSinglePlayerTouch, this));
	auto multiItem = MenuItemLabel::create(multi, CC_CALLBACK_1(MenuScene::onMultiPlayerTouch, this));

	auto menu = Menu::create(singleItem, multiItem, nullptr);
	menu->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 3);
	menu->alignItemsVerticallyWithPadding(10.0f);
	this->addChild(menu);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MenuScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(MenuScene::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("levelstarting.wav", false);

	return true;
}

void MenuScene::onKeyPressed(EventKeyboard::KeyCode keycode, Event * event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_ENTER)
	{
		Director::getInstance()->replaceScene(WaitingScene::createScene());
	}
}

void MenuScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event * event)
{
}

void MenuScene::onSinglePlayerTouch(Ref * node)
{
	Director::getInstance()->replaceScene(SinglePlayScene::createScene());
}

void MenuScene::onMultiPlayerTouch(Ref * node)
{
	auto playMulti = new IntegerPacket();
	playMulti->integerType = IntegerPacket::BEGIN_PLAY;
	playMulti->value = 0;

	ServerConnector::getInstance()->send(playMulti);
	delete playMulti;

	Director::getInstance()->replaceScene(WaitingScene::createScene());
}
