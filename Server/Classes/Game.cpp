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

	_isInGame = true;

	// add scene mới
	SceneManager::getInstance()->addScene(new PlayScene());

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
		//if (_detalTime >= _frameRate * 2)
		//	_detalTime = _frameRate;

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

	// data tự delete nếu add vào scene
	SceneManager::getInstance()->getCurrentScene()->handleData(data);
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

bool Game::isInGame()
{
	return _isInGame;
}

void Game::setInGame(bool value)
{
	_isInGame = value;
}

void Game::destroy() 
{
	SceneManager::getInstance()->destroy();
	delete SceneManager::getInstance();

	delete _gameTime;
	delete _factory;
}