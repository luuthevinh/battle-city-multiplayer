#include "InfoLayer.h"
#include "MenuScene.h"

InfoLayer::InfoLayer()
{
}

InfoLayer::~InfoLayer()
{
}

bool InfoLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(96, 96, 96, 255)))
	{
		return false;
	}

	this->setContentSize(Size(96, 512));

	_numberOfWaveColumns = 2;
	_currentWave = 0;

	auto wave = Label::createWithTTF("WAVES", "/fonts/pixel.ttf", 18.0f);
	wave->setAnchorPoint(Vec2(0.5f, 0.5f));
	wave->setColor(Color3B::BLACK);
	wave->setPosition(this->getContentSize().width / 2, 472.0f);
	this->addChild(wave);

	this->addWave(10);

	//auto player01 = Sprite::createWithSpriteFrameName("info_83.png");
	//player01->setAnchorPoint(Vec2(0.0f, 1.0f));
	//player01->setPosition(24.0f, 224.0f);
	//this->addChild(player01);

	auto icon01 = Sprite::createWithSpriteFrameName("tank_01.png");
	icon01->setAnchorPoint(Vec2(0.0f, 1.0f));
	icon01->setPosition(20.0f, 208.0f);
	this->addChild(icon01);

	_playerOneLife = Label::createWithTTF("3", "/fonts/pixel.ttf", 27.0f);
	_playerOneLife->setAnchorPoint(Vec2(0.0f, 1.0f));
	_playerOneLife->setColor(Color3B::BLACK);
	_playerOneLife->setPosition(Vec2(icon01->getPositionX() + 32.0f, icon01->getPositionY() + 1.0f));
	this->addChild(_playerOneLife);

	//auto player02 = Sprite::createWithSpriteFrameName("info_89.png");
	//player02->setAnchorPoint(Vec2(0.0f, 1.0f));
	//player02->setPosition(24.0f, 176.0f);
	//this->addChild(player02);

	auto icon02 = Sprite::createWithSpriteFrameName("tank_green_01.png");
	icon02->setAnchorPoint(Vec2(0.0f, 1.0f));
	icon02->setPosition(20.0f, 160.0f);
	this->addChild(icon02);

	_playerTwoLife = Label::createWithTTF("3", "/fonts/pixel.ttf", 27.0f);
	_playerTwoLife->setColor(Color3B::BLACK);
	_playerTwoLife->setAnchorPoint(Vec2(0.0f, 1.0f));
	_playerTwoLife->setPosition(Vec2(icon02->getPositionX() + 32.0f, icon02->getPositionY() + 1.0f));
	this->addChild(_playerTwoLife);


	auto menuLabel = Label::createWithTTF("MENU", "/fonts/pixel.ttf", 18.0f);
	menuLabel->setColor(Color3B::BLACK);
	auto menuItem = MenuItemLabel::create(menuLabel, CC_CALLBACK_1(InfoLayer::onMenuTouch, this));

	auto menu = Menu::create(menuItem, nullptr);
	menu->setPosition(this->getContentSize().width / 2, 96.0f);
	menu->alignItemsVerticallyWithPadding(10.0f);
	this->addChild(menu);

	return true;
}

void InfoLayer::update(float dt)
{
}

void InfoLayer::addWave(int number)
{
	Vec2 offset = Vec2(40.0f, 384.0f);

	for (size_t i = 0; i < number; i++)
	{
		auto r = _currentWave / _numberOfWaveColumns;
		auto c = _currentWave % _numberOfWaveColumns;

		auto sprite = Sprite::createWithSpriteFrameName("info_94.png");
		sprite->setPosition(offset.x + c * 16, offset.y + r * 16);
		sprite->setTag(_currentWave);
		this->addChild(sprite);

		_currentWave++;
	}

}

void InfoLayer::removeWave(int number)
{
	for (size_t i = 0; i < number; i++)
	{
		auto sprite = this->getChildByTag(_currentWave);
		sprite->removeFromParentAndCleanup(true);

		_currentWave--;
	}
}

void InfoLayer::setPlayerLife(int player, int life)
{
	switch (player)
	{
	case 1:
	{
		_playerOneLife->setString(StringUtils::format("%d", life));
		break;
	}
	case 2:
	{
		_playerTwoLife->setString(StringUtils::format("%d", life));
		break;
	}
	default:
		break;
	}
}

void InfoLayer::onMenuTouch(Ref * node)
{
	Director::getInstance()->replaceScene(MenuScene::createScene());
}
