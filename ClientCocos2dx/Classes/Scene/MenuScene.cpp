#include "MenuScene.h"
#include "WaitingScene.h"
#include "Base\SpriteManager.h"

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

	auto joinServer = Label::createWithTTF("JOIN SERVER", "/fonts/pixel.ttf", 15.0f);
	joinServer->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(joinServer);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MenuScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(MenuScene::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

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
