#include "TankBot.h"

#include "..\Shared\Utils.h"

TankBot::TankBot() : Tank(eObjectId::GREEN_TANK)
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

	this->findWay();

	this->setPosition(_nextPostions.front());
	_nextPostions.pop();
	
}

void TankBot::update(float dt)
{
	if (!_nextPostions.empty())
	{

		float x = tank::lerp(_nextPostions.front().x, this->getPosition().x, TANK_NORMAL_VELOCITY * dt);
		float y = tank::lerp(_nextPostions.front().y, this->getPosition().y, TANK_NORMAL_VELOCITY * dt);

		this->setPosition(x, y);

		this->onChanged();


		if (this->getPosition() == _nextPostions.front())
			_nextPostions.pop();
	}
}

void TankBot::setMap(tank::AStarMap* map)
{
	_aStarMap = map;
}

void TankBot::onContactBegin(GameObject & object)
{
	while (!_nextPostions.empty())
	{
		_nextPostions.pop();
	}

	this->findWay();
}

void TankBot::findWay()
{
	if (_aStarMap)
	{
		auto begin = _aStarMap->getNode(0, 0);
		auto end = _aStarMap->getNode(5, 4);
		auto result = _aStarMap->findPath(tank::Point(this->getPosition().x, this->getPosition().y), tank::Point(12*16, 15*16));

		for (int i = result.size() - 1; i >= 0; i--)
		{
			auto index = result[i]->getIndex();
			_nextPostions.push(Vector2(16 * (index.x + 1), 512 - 16 * (index.y + 1)));
		}
	}
}
