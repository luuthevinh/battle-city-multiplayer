#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
	while (!_gameObjects.empty())
	{
		delete _gameObjects.back();
		_gameObjects.pop_back();
	}

	while (!_players.empty())
	{
		delete _players.back();
		_players.pop_back();
	}
}

bool Game::init()
{
	_frameRate = 1.0f / GAME_FRAMERATE;

	return true;
}

void Game::update(float dt)
{
	for(auto object : _gameObjects)
	{
		object->update(dt);
	}

	for (auto player : _players)
	{
		player->update(dt);
	}
}

void Game::update(char * data)
{
}

float Game::getFrameRate()
{
	return _frameRate;
}

const std::vector<GameObject*>& Game::getAllGameObjects() const
{
	return _gameObjects;
}

const std::vector<Player*>& Game::getAllPlayers() const
{
	// TODO: insert return statement here
	return _players;
}

void Game::addPlayer(int index)
{
	auto player = new Player(eObjectId::YELLOW_TANK, index);
	player->setTag(index);

	_players.push_back(player);
}

void Game::removePlayer(int index)
{
	for (auto i = 0; i < _players.size(); i++)
	{
		if (_players[i]->getIndex() == index)
		{
			_players.erase(_players.begin() + i);
		}
	}
}

Player* Game::getPlayer(int index)
{
	for (auto player : _players)
	{
		if (player->getIndex() == index)
			return player;
	}

	return nullptr;
}
