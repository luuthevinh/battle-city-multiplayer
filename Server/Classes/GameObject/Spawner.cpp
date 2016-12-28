#include "Spawner.h"
#include "TankBot.h"

#include "..\Scene\PlayScene.h"
#include "..\Base\GameObject.h"

Spawner::Spawner(eObjectId type, PlayScene * parent)
{
	_objectType = type;
	_parentScene = parent;
	_counter = 0;
}

Spawner::~Spawner()
{
}

bool Spawner::init()
{
	return true;
}

void Spawner::update(float dt)
{
	if (_counter == 0)
		return;

	if (_timer > 0.0f)
	{
		_timer -= dt;
	}
	else 
	{
		_timer = _timeSpan;

		_counter--;
		this->createObjectAndAddToParent();
	}
}

void Spawner::setTime(float time)
{
	_timeSpan = time;
	_timer = time;
}

float Spawner::getTime()
{
	return _timeSpan;
}

void Spawner::setTotalObjects(int number)
{
	_total = number;
	_counter = _total;
}

int Spawner::getTotalObjects()
{
	return _total;
}

GameObject * Spawner::createObject()
{
	switch (_objectType)
	{
		case YELLOW_TANK:

		case GREEN_TANK:
			break;
		case WHITE_TANK:
		{
			auto tankbot = new TankBot();
			tankbot->setMap(_parentScene->getMap());
			tankbot->init();

			auto index = _parentScene->getRandomPositionIndex();
			tankbot->setPosition(_parentScene->getBotStartPosition(index));
			tankbot->setStartPositionIndex(index);
			tankbot->setTankLevel(eTankLevel::BASIC_TANK);

			return tankbot;
		}
	}
	return nullptr;
}

void Spawner::createObjectAndAddToParent()
{
	if (_parentScene == nullptr)
		return;

	auto object = this->createObject();
	_parentScene->addObject(object);
}
