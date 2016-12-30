#include "Player.h"
#include "Bullet.h"
#include "Base\SpriteManager.h"
#include "Base\ServerConnector.h"
#include "GameObject\Explosion.h"
#include "GameObject\AppearanceEffect.h"
#include "GameObject\ProtectionEffect.h"

// shared
#include "..\Server\Classes\Shared\DataPacket.h"
#include "..\Server\Classes\Shared\Utils.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

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

Player * Player::createInfo(Buffer & data)
{
	Player* tank = new(std::nothrow) Player(eObjectId::YELLOW_TANK);
	
	if (tank)
	{
		tank->initWithBuffer(data);

		data.setBeginRead(GameObject::INDEX_POSITION_X_BUFFER);
		float x = data.readFloat();
		float y = data.readFloat();
		tank->setPosition(x, y);

		return tank;
	}

	delete tank;
	return nullptr;
}

Player * Player::createGameObject(GameObject* object)
{
	Player* player = new(std::nothrow) Player(object->getId());
	if (player && player->init())
	{
		auto data = object->serialize();
		player->deserialize(*data);

		data->setBeginRead(GameObject::INDEX_POSITION_X_BUFFER);
		float x = data->readFloat();
		float y = data->readFloat();
		player->setPosition(x, y);

		player->autorelease();

		return player;
	}
	
	CC_SAFE_DELETE(player);
	return nullptr;
}

bool Player::init()
{
	if (!Tank::init())
	{
		return false;
	}

	_keyDirectionCounter = 0;
	_life = 3;

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
		/*ServerConnector::getInstance()->send(_commandQueue.front());*/
	}
	
	this->syncPositionWithLastUpdate(dt);

	this->fixWithBounding();

	if (this->hasStatus(eStatus::RUNNING))
	{
		if (_movingSoundId == 0)
		{
			_movingSoundId = SimpleAudioEngine::getInstance()->playEffect("nmoving.wav", true);
		}
	}
	else
	{
		if (_movingSoundId != 0)
		{
			SimpleAudioEngine::getInstance()->stopEffect(_movingSoundId);
			_movingSoundId = 0;
		}
	}
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
			_velocity = this->getVelocityByLevel();
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
			this->shoot();
			delete command;
		}
		else
		{
			_commandQueue.push(command);
		}


		//CCLOG("shoot: %.2f, %.2f", this->getPositionX(), this->getPositionY());
	}

	// test
	if (keycode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		auto number = (_tankLevel + 1);
		if (number > 8)
		{
			number = 1;
		}

		_tankLevel = (eTankLevel)number;
		this->updateSprite();
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
	//Tank::reconcile(data);

	this->deserialize(data);

	data.setBeginRead(GameObject::INDEX_POSITION_X_BUFFER);
	float x = data.readFloat();
	float y = data.readFloat();

	_lastUpdatedPosition = Vec2(x, y);
	_enableSync = true;

	this->setPosition(_lastUpdatedPosition);
	//float xx = tank::lerp(_lastUpdatedPosition.x, this->getPosition().x, 1.0f);
	//float yy = tank::lerp(_lastUpdatedPosition.y, this->getPosition().y, 1.0f);

	//this->setPosition(xx, yy);
}

void Player::updateWithCommand(CommandPacket * commad, float dt)
{
	Tank::updateWithCommand(commad, dt);
	
	//if (!this->hasStatus(eStatus::RUNNING))
	//	return;

	//auto curTime = ServerConnector::getInstance()->getClientTime();
	//auto interpolatedTime = curTime - 0.1f;

	//int from, to;
	//this->getFromToBufferIndex(from, to, interpolatedTime);

	//this->interpolate(*_previousBuffers.at(from), *_previousBuffers.at(to), interpolatedTime);

	//float x = tank::lerp(_nextPosition.x, _lastUpdatedPosition.x, TANK_NORMAL_VELOCITY * dt);
	//float y = tank::lerp(_nextPosition.y, _lastUpdatedPosition.y, TANK_NORMAL_VELOCITY * dt);

	//_lastUpdatedPosition.x = x;
	//_lastUpdatedPosition.y = y;
}

void Player::syncPositionWithLastUpdate(float dt)
{
	//if (!this->hasStatus(eStatus::RUNNING) || !_enableSync)
	//	return;

	//if (_lastUpdatedPosition != this->getPosition())
	//{
	//	float x = tank::lerp(_lastUpdatedPosition.x, this->getPosition().x, TANK_NORMAL_VELOCITY * dt);
	//	float y = tank::lerp(_lastUpdatedPosition.y, this->getPosition().y, TANK_NORMAL_VELOCITY * dt);

	//	this->setPosition(x, y);
	//}
	//else
	//{
	//	_enableSync = false;
	//}
}

void Player::addPendingCommand(CommandPacket * command)
{
	_pendingCommands[ServerConnector::getInstance()->getTime()] = *command;
}

void Player::predict(float dt)
{
	if (!_commandQueue.empty())
	{
		ServerConnector::getInstance()->send(_commandQueue.front());

		this->updateWithCommand(_commandQueue.front(), dt);

		if (_commandQueue.size() == 1 && _commandQueue.front()->begin)
		{
			return;
		}

		delete _commandQueue.front();
		_commandQueue.pop();
	}
}

void Player::move(eDirection direction, float dt)
{
	this->setDirection(direction);

	if (this->isCollidingAtSide(direction))
		return;

	if (!this->hasStatus(eStatus::RUNNING))
		return;

	this->fixPositionForTurn();

	switch (_direction)
	{
	case LEFT:
		this->setPositionX(this->getPositionX() - _velocity * dt);
		break;
	case UP:
		this->setPositionY(this->getPositionY() + _velocity * dt);
		break;
	case RIGHT:
		this->setPositionX(this->getPositionX() + _velocity * dt);
		break;
	case DOWN:
		this->setPositionY(this->getPositionY() - _velocity * dt);
		break;
	default:
		break;
	}

	this->onChanged();
}

void Player::updateWithStatus(eStatus status)
{
	switch (status)
	{
	case DIE:
	{
		_life--;
		if (_life > 0)
		{
			this->setVisible(false);
		}
		else
		{
			this->runAction(RemoveSelf::create());
		}

		if (this->getParent() == nullptr)
			break;
		
		auto explosion = Explosion::create(true);
		explosion->setPosition(this->getPosition());
		this->getParent()->addChild(explosion);
		
		break;
	}
	default:
		break;
	}
}

void Player::revive()
{
	auto appear = CallFunc::create([&] {
		this->setVisible(true);

		auto protection = ProtectionEffect::create();
		protection->setName("protection_effect");
		this->addChild(protection);
	});

	for (auto it = _bulletsRef.begin(); it != _bulletsRef.end(); it++)
	{
		it->second->setOwner(nullptr);
	}

	_bulletsRef.clear();
	_bulletOrder.clear();

	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), appear));
}

int Player::getLife()
{
	return _life;
}