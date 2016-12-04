#include "ServerPlayScene.h"
#include "GameObject\Player.h"
#include "Base\ServerConnector.h"

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

	//auto player = Player::create(eObjectId::YELLOW_TANK);
	//player->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	//player->setName("player");

	//this->addChild(player);

	// update
	this->scheduleUpdate();

	return true;
}

void ServerPlayScene::update(float dt)
{
	ServerConnector::getInstance()->update(dt);

	ServerConnector::getInstance()->handleData(this);
}
