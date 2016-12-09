#include "TankCursor.h"
#include "Base\SpriteManager.h"
#include "Base\ServerConnector.h"

#include "..\Server\Classes\Shared\DataPacket.h"

TankCursor::TankCursor()
{
}

TankCursor::~TankCursor()
{
}

bool TankCursor::init()
{
	if (!this->initWithSpriteFrameName("tank_07.png"))
	{
		return false;
	}

	_currentIndex = 0;

	_animations[eObjectId::YELLOW_TANK] = SpriteManager::getInstance()->getAnimate("yellow_tank_right");
	_animations[eObjectId::GREEN_TANK] = SpriteManager::getInstance()->getAnimate("green_tank_right");
	
	for (auto i = _animations.begin(); i != _animations.end(); i++)
	{
		i->second->retain();
	}

	this->runAction(RepeatForever::create(_animations[eObjectId::YELLOW_TANK]));

	// listener
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(TankCursor::onKeyPressed, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	return true;
}

void TankCursor::update(float dt)
{
}

void TankCursor::addPosition(int index, const Vec2& position)
{
	_indexPositions[index] = position;

	this->updatePositionWithIndex();
}

void TankCursor::onKeyPressed(EventKeyboard::KeyCode keycode, Event * e)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	{
		_currentIndex--;
		break;
	}
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	{
		_currentIndex++;
		break;
	}
	case EventKeyboard::KeyCode::KEY_ENTER:
	{
		auto packet = new IntegerPacket();
		packet->integerType = IntegerPacket::Type::PLAYER_CHARACTER_SELECTION;
		packet->value = _currentIndex;
		packet->setUniqueId(ServerConnector::getInstance()->getUniqueId());

		ServerConnector::getInstance()->send(packet);

		delete packet;

		break;
	}
	default:
		break;
	}

	if (_currentIndex < 0)
	{
		_currentIndex = _indexPositions.size() - 1;
	}

	if (_currentIndex >= _indexPositions.size())
	{
		_currentIndex = 0;
	}

	this->updatePositionWithIndex();
}

int TankCursor::getCurrentIndex()
{
	return _currentIndex;
}

void TankCursor::updatePositionWithIndex()
{
	if (_indexPositions.find(_currentIndex) == _indexPositions.end())
		return;

	this->setPosition(_indexPositions[_currentIndex]);

	this->stopAllActions();

	switch (_currentIndex)
	{
	case 0:
	{
		this->runAction(RepeatForever::create(_animations[eObjectId::YELLOW_TANK]));
		break;
	}
	case 1:
	{
		this->runAction(RepeatForever::create(_animations[eObjectId::GREEN_TANK]));
		break;
	}
	default:
		break;
	}
}
