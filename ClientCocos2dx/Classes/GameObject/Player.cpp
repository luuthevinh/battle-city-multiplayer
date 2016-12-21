#include "Player.h"
#include "Bullet.h"
#include "Base\SpriteManager.h"
#include "Base\ServerConnector.h"

// shared
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
	this->predict(dt);

	if (!_commandQueue.empty())
	{
		ServerConnector::getInstance()->send(_commandQueue.front());
	}

	//if (_nextPosition != Vec2::ZERO)
	//{
	//	if(_direction == eDirection::UP || _direction == eDirection::DOWN)
	//		this->setPositionX(tank::lerp(_nextPosition.x, this->getPositionX(), TANK_NORMAL_VELOCITY * dt));

	//	if (_direction == eDirection::LEFT || _direction == eDirection::RIGHT)
	//		this->setPositionY(tank::lerp(_nextPosition.y, this->getPositionY(), TANK_NORMAL_VELOCITY * dt));
	//}
}

void Player::onKeyPressed(EventKeyboard::KeyCode keycode, Event * e)
{
	eKeyInput input = eKeyInput::KEY_NONE;

	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	{
		this->setDirection(eDirection::UP);
		input = eKeyInput::KEY_UP;
		break;
	}
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	{
		this->setDirection(eDirection::DOWN);
		input = eKeyInput::KEY_DOWN;
		break;
	}
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	{
		this->setDirection(eDirection::LEFT);
		input = eKeyInput::KEY_LEFT;
		break;
	}
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	{
		this->setDirection(eDirection::RIGHT);
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
			_velocity = TANK_NORMAL_VELOCITY;
			this->addStatus(eStatus::RUNNING);

			this->onChanged();
		}

		auto command = new CommandPacket();
		command->input = input;
		command->setUniqueId(this->getTag());
		command->begin = true;

		if (input == eKeyInput::KEY_SHOOT)
		{
			ServerConnector::getInstance()->send(command);
		}
		else
		{
			_commandQueue.push(command);
		}


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
		_velocity = 0.0f;
		this->removeStatus(eStatus::RUNNING);
		_sprite->stopAllActions();

		this->onChanged();
	}

	if (input != eKeyInput::KEY_NONE)
	{
		auto command = new CommandPacket();
		command->input = input;
		command->setUniqueId(this->getTag());
		command->begin = false;

		_commandQueue.emplace(command);

	}
}

void Player::reconcile(Buffer & data)
{
	GameObject::reconcile(data);

	data.setBeginRead(GameObject::INDEX_POSITION_X_BUFFER);
	float x = data.readFloat();
	float y = data.readFloat();
	Vec2 actualPosition = Vec2(x, y);

	this->setPosition(actualPosition);

	//this->updateLastBuffer(data);
	//this->deserialize(data);

	//if (_pendingBuffer.size() <= 0)
	//{
	//	return;
	//}

	//if (_previousBuffer != nullptr)
	//{ 
	//	this->interpolate();
	//}

	//if (_previousBuffer != nullptr)
	//{ 
	//	this->interpolate();

	//	data.setBeginRead(data.getSize() - sizeof(int) * 3);
	//	float x = data.readFloat();
	//	float y = data.readFloat();
	//	Vec2 actualPosition = Vec2(x, y);
	//	Vec2 actualDistance = actualPosition - _lastPosition;

	//	if (actualDistance == _deltaDistance)
	//	{
	//		_movedDistancePredition = Vec2::ZERO;
	//	}
	//	else
	//	{
	//		_deltaDistance = actualDistance;
	//	}
	//	
	//	this->setPosition(actualPosition);
	//}

	//auto time = ServerConnector::getInstance()->getTime();

	//int index = 0;
	//for (index; index < _pendingBuffer.size(); index++)
	//{
	//	_pendingBuffer[index]->setBeginRead(_pendingBuffer[index]->getSize() - 4);
	//	auto t = _pendingBuffer[index]->readFloat();

	//	// thời gian nhận được sau pending thì cập nhật lại từ đây
	//	if (time <= t)
	//	{
	//		this->deserialize(*_pendingBuffer[index]);
	//		CCLOG("pending[%d]: %.2f | time: %.2f", index, t, time);
	//	}
	//}

	//while (!_pendingBuffer.empty())
	//{
	//	delete _pendingBuffer.front();
	//	_pendingBuffer.pop_front();
	//}
}

void Player::updateWithCommand(CommandPacket * commad, float dt)
{
	Tank::updateWithCommand(commad, dt);

	//this->addToPendingBuffer();
}

void Player::predict(float dt)
{
	if (!_commandQueue.empty())
	{
		this->updateWithCommand(_commandQueue.front(), dt);

		if (_commandQueue.size() == 1 && _commandQueue.front()->begin)
		{
			return;
		}

		delete _commandQueue.front();
		_commandQueue.pop();

		//_movedDistancePredition += this->getPosition() - _lastPosition;
	}
}
