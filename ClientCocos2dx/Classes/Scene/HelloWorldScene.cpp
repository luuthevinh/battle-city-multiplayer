#include "HelloWorldScene.h"
#include "Base\SpriteManager.h"

USING_NS_CC;

HelloWorld* HelloWorld::instance = nullptr;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
	SpriteManager::getInstance()->init();
    
	_player = Player::create(eObjectId::YELLOW_TANK);
	_player->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(_player);

	_otherPlayer = Tank::create(eObjectId::YELLOW_TANK);
	_otherPlayer->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(_otherPlayer);

	// init server
	_serverConnector = new ServerConnector();
	if (_serverConnector->init(PORT, SERVER_ADD))
	// connect to server
	_serverConnector->connectServer();

	// update
	this->scheduleUpdate();

	//
	this->instance = this;

    return true;
}

void HelloWorld::update(float dt)
{
	_serverConnector->update(this);
}

ServerConnector * HelloWorld::getConnector()
{
	return _serverConnector;
}
