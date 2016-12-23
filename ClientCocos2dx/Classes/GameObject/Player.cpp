#include "Player.h"
#include "Bullet.h"
#include "Base\SpriteManager.h"
#include "Base\ServerConnector.h"


// shared
#include "..\Server\Classes\Shared\DataPacket.h"
#include "..\Server\Classes\Shared\Utils.h"

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

Player * Player::createWithBuffer(Buffer & data)
{
	Player* tank = new(std::nothrow) Player(eObjectId::YELLOW_TANK);
	
	if (tank && tank->init())
	{
		tank->initWithBuffer(data);

		data.setBeginRead(GameObject::INDEX_POSITION_X_BUFFER);
		float x = data.readFloat();
		float y = data.readFloat();
		tank->setPosition(x, y);

		tank->autorelease();
		return tank;
	}

	delete tank;
	return nullptr;
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

	_enableSync = false;

	return true;
}

void Player::update(float dt)
{
	this->predict(dt);

	if (!_commandQueue.empty())
	{
		ServerConnector::getInstance()->send(_commandQueue.front());
	}

	this->syncPositionWithLastUpdate(dt);
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

	_lastUpdatedPosition = Vec2(x, y);

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

	//_pendingBuffer.clear();
}

void Player::updateWithCommand(CommandPacket * commad, float dt)
{
	// Tank::updateWithCommand(commad, dt);

	if (!this->hasStatus(eStatus::RUNNING) || _nextPosition == Vec2::ZERO)
		return;

	float x = tank::lerp(_nextPosition.x, _lastUpdatedPosition.x, TANK_NORMAL_VELOCITY * dt);
	float y = tank::lerp(_nextPosition.y, _lastUpdatedPosition.y, TANK_NORMAL_VELOCITY * dt);

	_lastUpdatedPosition.x = x;
	_lastUpdatedPosition.y = y;
}

void Player::syncPositionWithLastUpdate(float dt)
{
	if (!this->hasStatus(eStatus::RUNNING))
		return;

	if (_lastUpdatedPosition != this->getPosition() && !_firstUpdated)
	{
		float x = tank::lerp(_lastUpdatedPosition.x, this->getPosition().x, TANK_NORMAL_VELOCITY * dt);
		float y = tank::lerp(_lastUpdatedPosition.y, this->getPosition().y, TANK_NORMAL_VELOCITY * dt);

		this->setPosition(x, y);
	}
}

void Player::addPendingCommand(CommandPacket * command)
{
	_pendingCommands[ServerConnector::getInstance()->getTime()] = *command;
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
	}
}
