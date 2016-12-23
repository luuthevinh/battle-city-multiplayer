#include "Game.h"
#include "Base\SceneManager.h"
#include "Scene\PlayScene.h"
#include "Base\ServerConverterFactory.h"
#include "Server.h"

Game* Game::instance = nullptr;

Game * Game::createWithConverter(ConverterFactory * factory)
{
	Game* ret = new Game(factory);
	if (ret->init())
	{
		return ret;
	}

	delete ret;
	return nullptr;
}

Game::Game(ConverterFactory* factory)
{
	_factory = factory;
}

Game::~Game()
{
}

bool Game::init()
{
	_frameRate = 1.0f / GAME_FRAMERATE;

	_detalTime = 0;
	_lastTime = 0;

	// game time
	_gameTime = new GameTime();
	_gameTime->init();

	instance = this;

	// add scene mới
	SceneManager::getInstance()->addScene(new PlayScene());

	_maxBots = MAX_NUMBER_OF_BOTS;
	_totalBots = 0;

	return true;
}

void Game::run()
{
	_gameTime->update();

	_detalTime = _gameTime->getTotalTime() - _lastTime;

	if (_detalTime >= this->getFrameRate())
	{
		_lastTime += this->getFrameRate();

		// data
		this->handleData();

		// lock frame ko cho dồn nhiều
		if (_detalTime >= _frameRate * 2)
			_detalTime = _frameRate;

		// update game
		this->update(_detalTime);

		// title console
		float f = 1.0f / _detalTime;
		char buffer[100];
		sprintf(buffer, "(%.1f/s) | %.4f | Tank Server \n", f, _detalTime);
		SetConsoleTitle(buffer);
	}
	else
	{
		Sleep((this->getFrameRate() - _detalTime) * 1000.0f);
	}
}

void Game::update(float dt)
{
	// cập nhật scene
	SceneManager::getInstance()->update(dt);
}

float Game::getFrameRate()
{
	return _frameRate;
}

void Game::handleData()
{
	// update object with recieved data
	auto data = _factory->convertNext();
	if (data == nullptr)
		return;
	
	switch (data->getType())
	{
	case eDataType::COMMAND:
	{
		// data tự delete nếu add vào scene
		SceneManager::getInstance()->getCurrentScene()->handleData(data);
		return;
	}
	default:
		break;
	}

	// data sẽ ko delete
	this->handleDataFromWaitingRoom(data);
	if(data != nullptr)
		delete data;
}

void Game::handleDataFromWaitingRoom(Serializable * data)
{
	auto type = data->getType();

	switch (type)
	{
	case INTEGER:
	{
		this->handleIntegerPacket((IntegerPacket*)data);
		break;
	}
	default:
		break;
	}
}

void Game::handleIntegerPacket(IntegerPacket * integer)
{
	auto integerType = integer->integerType;
	switch (integerType)
	{
	case IntegerPacket::PLAYER_CHARACTER_SELECTION:
	{
		auto player = Server::instance->getClientManager()->getPlayerByUniqueId(integer->getUniqueId());
		if (player)
		{
			player->setId((eObjectId)integer->value);
			Server::instance->getClientManager()->onChanged();
		}
		break;
	}
	case IntegerPacket::READY:
		break;
	case IntegerPacket::BEGIN_PLAY:
	{
		auto player = Server::instance->getClientManager()->getPlayerByUniqueId(integer->getUniqueId());
		if (player)
		{
			this->createPlayer(*player);

			auto to = Server::instance->getClientManager()->getClientSocket(player->getIndex());
			Server::instance->sendTo(to, player);

			if (player->isHost())
			{
				SceneManager::getInstance()->getCurrentScene()->beginGame();
			}
		}
		break;
	}
	case IntegerPacket::SET_BOT:
	{
		auto player = Server::instance->getClientManager()->getPlayerByUniqueId(integer->getUniqueId());
		if (player && player->isHost())
		{
			this->updateBots(integer->value);
		}
		break;
	}
	default:
		break;
	}
}

void Game::setConverterFactory(ConverterFactory * factory)
{
	if (_factory != nullptr)
		delete _factory;

	_factory = factory;
}

GameTime * Game::getGameTime()
{
	return _gameTime;
}

int Game::getNumberOfBots()
{
	return _totalBots;
}

void Game::updateBots(int value)
{
	_totalBots += value;

	if (_totalBots < 0)
		_totalBots = _maxBots;

	if (_totalBots > _maxBots)
		_totalBots = 0;

	Server::instance->getClientManager()->onChanged();
}

void Game::createPlayer(const Player & info)
{
	auto player = new Player(info.getId(), info.getIndex());
	player->setHost(info.isHost());
	player->setUniqueId(info.getUniqueId());

	SceneManager::getInstance()->getCurrentScene()->addPlayer(player);
}

void Game::destroy() 
{
	SceneManager::getInstance()->destroy();
	delete SceneManager::getInstance();

	delete _gameTime;
	delete _factory;
}