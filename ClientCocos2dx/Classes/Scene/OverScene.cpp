#include "OverScene.h"
#include "Scene\WaitingScene.h"
#include "Scene\MenuScene.h"
#include "Base\ServerConnector.h"
#include "Base\ClientConverterFactory.h"

#include "..\Server\Classes\Shared\DataPacket.h"

Scene * OverScene::createScene()
{
	auto scene = Scene::create();

	auto layer = OverScene::create();
	scene->addChild(layer);

	return scene;
}

OverScene::OverScene()
{
}

OverScene::~OverScene()
{
}

bool OverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto playAgain = Label::createWithTTF("PLAY AGAIN", "/fonts/pixel.ttf", 18.0f);
	auto menuLabel = Label::createWithTTF("MENU", "/fonts/pixel.ttf", 18.0f);

	auto playAgainItem = MenuItemLabel::create(playAgain, CC_CALLBACK_1(OverScene::onPlayAgainTouch, this));
	auto menuItem = MenuItemLabel::create(menuLabel, CC_CALLBACK_1(OverScene::onMenuTouch, this));

	auto menu = Menu::create(playAgainItem, menuItem, nullptr);
	menu->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	menu->alignItemsVerticallyWithPadding(10.0f);
	
	this->addChild(menu);

	return true;
}

void OverScene::update(float dt)
{
	if (ServerConnector::getInstance()->isRunning())
	{
		ServerConnector::getInstance()->update(dt);
		this->handleData();
	}
}

void OverScene::handleData()
{
	if (!ServerConnector::getInstance()->isConnected())
		return;

	auto data = ServerConnector::getInstance()->getFactory()->convertNext();

	if (data == nullptr)
		return;

	auto type = data->getType();

}

void OverScene::onPlayAgainTouch(Ref * node)
{
	auto playAgain = new IntegerPacket();
	playAgain->integerType = IntegerPacket::BEGIN_PLAY;
	playAgain->value = 0;

	ServerConnector::getInstance()->send(playAgain);
	delete playAgain;

	Director::getInstance()->replaceScene(WaitingScene::createScene());
}

void OverScene::onMenuTouch(Ref * node)
{
	Director::getInstance()->replaceScene(MenuScene::createScene());
}
