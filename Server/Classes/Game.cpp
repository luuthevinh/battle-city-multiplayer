#include "Game.h"
#include "GameObject\Bullet.h"
#include "Server.h"
#include "Shared\DataPacket.h"

Game* Game::instance = nullptr;

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

	instance = this;
	_uniqueIdCounter = 0;

	return true;
}

void Game::update(float dt)
{
	for(auto object : _gameObjects)
	{
		object->update(dt);

		if (object->hasChanged())
		{
			Server::instance->send(object);
			object->setChanged(false);
		}
	}

	for (auto player : _players)
	{
		player->update(dt);

		if (player->hasChanged())
		{
			Server::instance->send(player);
			player->setChanged(false);
		}
	}

	// xóa object có status là die
	for (auto it = _gameObjects.begin(); it != _gameObjects.end(); )
	{
		if ((*it)->getStatus() == eStatus::DIE)
		{
			delete *it;
			it = _gameObjects.erase(it);
		}
		else
		{
			it++;
		}
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
	_uniqueIdCounter++;

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

int Game::addObject(GameObject* object)
{
	_uniqueIdCounter++;

	_gameObjects.push_back(object);
	object->setTag(_uniqueIdCounter);

	return _gameObjects.size();
}

void Game::handlePlayerInput(int playerTag, eKeyInput input, bool start)
{
	this->getPlayer(playerTag)->updateInput(input, start);
}

void Game::handleData(Serializable * object)
{
	auto type = object->getType();

	switch (type)
	{
	case OBJECT:
	{
		break;
	}
	case PACKET:
		break;
	case REPLY_ID:
		break;
	case COMMAND:
	{
		if (auto command = dynamic_cast<CommandPacket*>(object))
		{
			this->handlePlayerInput(command->uniqueId, command->input, command->begin);
		}
		break;
	}
	default:
		break;
	}
}

void Game::removeObject(int tag)
{
	for (auto it = _gameObjects.begin(); it != _gameObjects.end(); it++)
	{
		if ((*it)->getTag() == tag)
		{
			delete *it;
			_gameObjects.erase(it);
			return;
		}
	}
}