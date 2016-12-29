﻿#include "TankBot.h"
#include "..\Base\AABB.h"
#include <time.h>
#include <random>

#include "..\Shared\Utils.h"

TankBot::TankBot() : Tank(eObjectId::WHITE_TANK)
{
	this->init();
}

TankBot::~TankBot()
{
}

bool TankBot::init()
{
	if (!Tank::init())
	{
		return false;
	}

	this->setTankLevel(eTankLevel::BASIC_TANK);

	_isActive = false;
	_activeTimer = 1.0f;

	return true;
}

void TankBot::update(float dt)
{
	this->countingToActive(dt);

	if (!_isActive)
		return;

	auto decide = this->think();

	switch (decide)
	{
		case TankBot::SHOOT:
			this->shoot();
			break;
		case TankBot::MOVE_NEXT:
			break;
		case TankBot::FIND_NEW_WAY:
			this->findNewWay();
			break;
		default:
			break;
	}

	if (_nextPostions.size() > 1)
	{
		this->moveNext(dt);
	}
	else
	{
		this->addStatus(eStatus::RUNNING);
		_velocity = TANK_NORMAL_VELOCITY;
		this->findNewWay();
	}

	this->updateBoundingBoxPosition();
}

void TankBot::setMap(tank::AStarMap* map)
{
	_aStarMap = map;
}

void TankBot::onContactBegin(GameObject & object)
{
	if (object.getId() == this->getId())
	{
		this->removeStatus(eStatus::RUNNING);
		_velocity = 0;

		while (_nextPostions.size() > 1)
		{
			_nextPostions.pop();
		}

		auto curPosition = object.getPosition();
		auto otherIndex = _aStarMap->positionToIndex(tank::Point(curPosition.x, curPosition.y));

		this->findNewWayWithTempObstacle(otherIndex);
	}
}

void TankBot::checkCollision(GameObject & other, float dt)
{
	if (!_isActive)
		return;

	if (other.getId() == eObjectId::BULLET || other.getId() == eObjectId::GRASS_WALL)
	{
		return;
	}

	this->updateBoundingBoxPosition();

	eDirection result;
	float time = _collisionChecker->checkCollision(*this, other, result, dt);

	// có va chạm
	if (time < 1.0f)
	{
		this->onContactBegin(other);
	}
}

void TankBot::findNewWay()
{
	if (_aStarMap)
	{
		auto current = this->getPosition();
		if (_nextPostions.size() > 0)
		{
			current = _nextPostions.back();
		}

		auto target = this->getRandomNextPostion();
		auto index = _aStarMap->positionToIndex(target);
		// printf("Next target: (%d, %d) index (%d, %d)\n", target.x, target.y, index.x, index.y);

		auto result = _aStarMap->findPath(tank::Point(current.x, current.y), target);

		for (int i = result.size() - 1; i >= 0; i--)
		{
			auto index = result[i]->getIndexInPosition();
			_nextPostions.push(Vector2(index.x, index.y));
		}
	}
}

void TankBot::findNewWayWithTempObstacle(const tank::Point & index)
{
	_aStarMap->addTempObstacle(index);

	this->findNewWay();

	_aStarMap->clearTempObstacle();
}

void TankBot::moveNext(float dt)
{
	auto current = this->getPosition();
	auto nextPostion = _nextPostions.front();

	// direction
	auto delta = nextPostion - current;
	if (delta.x > 0)
	{
		this->setDirection(eDirection::RIGHT);
	}
	else if (delta.x < 0)
	{
		this->setDirection(eDirection::LEFT);
	}
	else if (delta.y < 0)
	{
		this->setDirection(eDirection::DOWN);
	}
	else if (delta.y > 0)
	{
		this->setDirection(eDirection::UP);
	}

	float y = tank::lerp(nextPostion.y, current.y, _velocity * dt);
	float x = tank::lerp(nextPostion.x, current.x, _velocity * dt);

	if (this->getPosition() != Vector2(x, y))
	{
		this->onChanged();
		this->setPosition(x, y);
	}

	auto lenght = (current - _oldPositon).lenght();
	auto target = (nextPostion - _oldPositon).lenght();

	if(current == nextPostion || target == 0)
	{
		// this->removeStatus(eStatus::RUNNING);
		_oldPositon = _nextPostions.front();
		_nextPostions.pop();
	}
}

void TankBot::addCommand(eKeyInput input, bool begin)
{
	auto command = new CommandPacket();
	command->begin = begin;
	command->input = input;

	_commandQueue.push(command);
}

void TankBot::countingToActive(float dt)
{
	if (_isActive)
		return;

	if (_activeTimer > 0.0f)
	{
		_activeTimer -= dt;
	}
	else
	{
		_activeTimer == 0.0f;
		_isActive = true;
	}
}

TankBot::eDecision TankBot::think()
{
	eDecision decide = eDecision::MOVE_NEXT;

	auto r = rand() % 100;
	switch (r)
	{
	case 1:
		decide = eDecision::SHOOT;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		decide = eDecision::FIND_NEW_WAY;
		break;
	default:
		break;
	}

	return decide;
}

Point TankBot::getRandomNextPostion()
{
	//auto randNode = _aStarMap->getRandomNodeHasValue(0);
	//return randNode->getIndexInPosition();
	
	int value = rand() % 7;

	switch (value)
	{
		case 0:
			return Point(1 * 16, (31 - 0) * 16);
		case 1:
			return Point(25 * 16, (31 - 0) * 16);
		case 2:
			return Point(13 * 16, (31 - 16) * 16);
		case 3:
			return Point(25 * 16, (31 - 30) * 16);
		case 4:
			return Point(1 * 16, (31 - 30) * 16);
		case 5:
			return Point(10 * 16, (31 - 30) * 16);
		case 6:
			return Point(16 * 16, (31 - 30) * 16);
		default:
			break;
	}
	
}
