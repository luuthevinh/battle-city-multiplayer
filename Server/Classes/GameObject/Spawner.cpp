#include "Spawner.h"
#include "TankBot.h"

#include "..\Scene\PlayScene.h"
#include "..\Base\GameObject.h"
#include "..\Server.h"
#include "..\Shared\DataPacket.h"

Spawner::Spawner(eObjectId type, PlayScene * parent)
{
	_objectType = type;
	_parentScene = parent;
	this->init();
}

Spawner::~Spawner()
{
}

bool Spawner::init()
{
	_counter = 0;
	_currentMax = 2;
	_waveCounter = 1;
	_canSpawn = true;
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

		if (_total == 1)
		{
			if (_canSpawn && _currentCounter < _currentMax)
			{
				_currentCounter++;
				this->createObjectAndAddToParent();

				if (_currentCounter == _currentMax)
				{
					_canSpawn = false;
				}
			}
		}
		else
		{
			if (_currentCounter < _currentMax)
			{
				_currentCounter++;
				_counter--;
				this->createObjectAndAddToParent();
			}
		}
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

void Spawner::setCurrent(int number)
{
	_currentCounter = number;
	if (_currentCounter == 0 && _total == 1)
	{
		this->nextWave();
	}
}

int Spawner::getCurrent()
{
	return _currentCounter;
}

void Spawner::setCurrentMax(int number)
{
	_currentMax = number;
}

eObjectId Spawner::getObjectType()
{
	return _objectType;
}

int Spawner::getWave()
{
	return _waveCounter;
}

void Spawner::nextWave()
{
	_canSpawn = true;
	_waveCounter++;
	_currentMax = 2;

	auto wave = new IntegerPacket(IntegerPacket::WAVE, _waveCounter);
	Server::instance->send(wave);
	delete wave;
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
			tankbot->setTankLevel(this->getTankLevel());

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

eTankLevel Spawner::getTankLevel()
{
	if (_waveCounter > 10)
		_waveCounter = 10;

	auto r = rand() % _waveCounter;

	switch (r)
	{
		case 0:
		case 1:
			return eTankLevel::BASIC_TANK;
		case 2:
		case 3:
		case 4:
			return eTankLevel::FAST_TANK;
		case 5:
		case 6:
		case 7:
			return eTankLevel::POWER_TANK;
		case 8:
		case 9:
			return eTankLevel::ARMOR_TANK;
		default:
			return eTankLevel::BASIC_TANK;
	}

	return eTankLevel::BASIC_TANK;
}
