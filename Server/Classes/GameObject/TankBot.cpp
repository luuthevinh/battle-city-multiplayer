#include "TankBot.h"
#include "..\Base\AABB.h"
#include <time.h>
#include <random>

#include "..\Shared\Utils.h"

TankBot::TankBot() : Tank(eObjectId::WHITE_TANK)
{
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

	return true;
}

void TankBot::update(float dt)
{
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
	if (!this->canCollisionWith(other.getCategoryBitmask()))
		return;

	if (other.getId() == eObjectId::BULLET)
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

Point TankBot::getRandomNextPostion()
{
	//auto randNode = _aStarMap->getRandomNodeHasValue(0);
	//return randNode->getIndexInPosition();
	
	int value = rand() % 5;

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
		default:
			break;
	}
	
}
