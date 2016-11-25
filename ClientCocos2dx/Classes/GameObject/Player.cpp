#include "Player.h"
#include "Base\SpriteManager.h"
#include "Scene\HelloWorldScene.h"
#include "Bullet.h"
#include "..\Server\Classes\Shared\DataPacket.h"

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

	//auto body = PhysicsBody::createBox(Size(26, 26), PhysicsMaterial(0, 0, 0));
	//body->setContactTestBitmask(1);

	//body->setDynamic(false);
	//this->setPhysicsBody(body);


	return true;
}

void Player::update(float dt)
{
	Tank::update(dt);
}

void Player::onKeyPressed(EventKeyboard::KeyCode keycode, Event * e)
{
	eKeyInput input = eKeyInput::KEY_NONE;

	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	{
		//this->setDirection(eDirection::UP);
		input = eKeyInput::KEY_UP;
		break;
	}
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	{
		//this->setDirection(eDirection::DOWN);
		input = eKeyInput::KEY_DOWN;
		break;
	}
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	{
		//this->setDirection(eDirection::LEFT);
		input = eKeyInput::KEY_LEFT;
		break;
	}
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	{
		//this->setDirection(eDirection::RIGHT);
		input = eKeyInput::KEY_RIGHT;
		break;
	}
	case EventKeyboard::KeyCode::KEY_X:
	{
		input = eKeyInput::KEY_SHOOT;
		break;
	}
	default:
		break;
	}

	if (input != eKeyInput::KEY_NONE)
	{
		if (input != eKeyInput::KEY_SHOOT)
		{
			_keyDirectionCounter++;
			//_velocity = TANK_NORMAL_VELOCITY;
			//_status = (eStatus)(_status | eStatus::RUNNING);
		}

		auto command = new CommandPacket();
		command->input = input;
		command->uniqueId = this->getTag();
		command->begin = true;

		HelloWorld::instance->getConnector()->send(command);

		delete command;

		//CCLOG("shoot: %.2f, %.2f", this->getPositionX(), this->getPositionY());
	}
}

void Player::onKeyReleased(EventKeyboard::KeyCode keycode, Event * e)
{
	eKeyInput input = eKeyInput::KEY_NONE;

	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	{
		_keyDirectionCounter--;
		input = eKeyInput::KEY_UP;
		break;
	}
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	{
		_keyDirectionCounter--;
		input = eKeyInput::KEY_DOWN;
		break;
	}
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	{
		_keyDirectionCounter--;
		input = eKeyInput::KEY_LEFT;
		break;
	}
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	{
		_keyDirectionCounter--;
		input = eKeyInput::KEY_RIGHT;
		break;
	}
	default:
		break;
	}

	if (_velocity != 0.0f && _keyDirectionCounter <= 0)
	{
		//_velocity = 0.0f;
		//_status = (eStatus)(_status & (~eStatus::RUNNING));
		_sprite->stopAllActions();

	}

	if (input != eKeyInput::KEY_NONE)
	{
		auto command = new CommandPacket();
		command->input = input;
		command->uniqueId = this->getTag();
		command->begin = false;

		HelloWorld::instance->getConnector()->send(command);

		delete command;
	}
}