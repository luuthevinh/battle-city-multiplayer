#include "Game.h"
#include "Base\SceneManager.h"
#include "Scene\Scene01.h"

Game* Game::instance = nullptr;

Game::Game()
{
}

Game::~Game()
{
}

bool Game::init()
{
	_frameRate = 1.0f / GAME_FRAMERATE;
	instance = this;

	// add scene mới
	SceneManager::getInstance()->addScene(new Scene01());

	return true;
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

void Game::handleData(ConverterFactory* factory)
{
	// update object with recieved data
	auto data = factory->convertNext();
	if (data == nullptr)
		return;

	SceneManager::getInstance()->getCurrentScene()->handleData(data);

	delete data;
}

void Game::destroy() 
{
	SceneManager::getInstance()->destroy();
	delete SceneManager::getInstance();
}