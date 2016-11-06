#include "Player.h"
#include "Base\SpriteManager.h"
#include "Scene\HelloWorldScene.h"

Player::Player(eObjectId id) : Tank(id)
{
}

Player::~Player()
{
}

Player* Player::create(eObjectId id)
{
	Player* player = new(std::nothrow) Player(id);
	if (player && player->init())
	{
		player->autorelease();
		return player;
	}
	else
	{
		delete player;
		player = nullptr;
		return nullptr;
	}
}

bool Player::init()
{
	if (!Tank::init())
	{
		return false;
	}

	_keyDirectionCounter = 0;

	// key board listener
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void Player::update(float dt)
{
	Tank::update(dt);

	//std::string info = "postion: ";
	//char buffer[100];
	//sprintf(buffer, "position: (%.2f, %.2f)", this->getPositionX(), this->getPositionY());
	//HelloWorld::instance->getConnector()->sendData(buffer);

	//ObjectPacket pack;
	//if(_velocity != 0)
	//	pack.direction = _direction;
	//else
	//	pack.direction = 0;
	//pack.id = 0;
	//pack.x = this->getPositionX();
	//pack.y = this->getPositionY();
	//pack.dx = 0;
	//pack.dy = 0;

	Packet pack;
	pack.packetType = Packet::TANK;
	if (_velocity != 0)
		pack.TankPacket.direction = _direction;
	else
		pack.TankPacket.direction = 0;

	pack.TankPacket.id = _id;
	pack.TankPacket.x = this->getPositionX();
	pack.TankPacket.y = this->getPositionY();
	pack.TankPacket.status = this->getStatus();

	HelloWorld::instance->getConnector()->sendData(pack);
}

void Player::setDirection(eDirection direction)
{
	if (_direction == direction)
		return;

	_direction = direction;

	_sprite->stopAllActions();

	if ((_status & eStatus::RUNNING) == eStatus::RUNNING)
	{
		_sprite->runAction(RepeatForever::create(_animations[_direction]));
	}
	else
	{
		_sprite->runAction(_animations[_direction]);
	}
}

void Player::onKeyPressed(EventKeyboard::KeyCode keycode, Event * e)
{
	if (_velocity == 0)
	{
		_velocity = TANK_NORMAL_VELOCITY;
		_status = (eStatus)(_status | eStatus::RUNNING);
	}


	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	{
		this->setDirection(eDirection::UP);
		_keyDirectionCounter++;

		break;
	}
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	{
		this->setDirection(eDirection::DOWN);
		_keyDirectionCounter++;
		break;
	}
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	{
		this->setDirection(eDirection::LEFT);
		_keyDirectionCounter++;
		break;
	}
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	{
		this->setDirection(eDirection::RIGHT);
		_keyDirectionCounter++;
		break;
	}
	default:
		break;
	}
}

void Player::onKeyReleased(EventKeyboard::KeyCode keycode, Event * e)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	{
		_keyDirectionCounter--;
		break;
	}
	default:
		break;
	}

	if (_velocity != 0.0f && _keyDirectionCounter <= 0)
	{
		_velocity = 0.0f;
		_status = (eStatus)(_status & (~eStatus::RUNNING));
		_sprite->stopAllActions();
	}
}