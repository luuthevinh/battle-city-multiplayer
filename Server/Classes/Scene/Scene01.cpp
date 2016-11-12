#include "Scene01.h"
#include "..\Server.h"

#include "..\Shared\DataPacket.h"

Scene01::Scene01()
{
}

Scene01::~Scene01()
{
}

bool Scene01::init()
{
	return true;
}

void Scene01::update(float dt)
{
	for (auto object : _gameObjects)
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

	// kiểm tra trạng thái object
	this->checkStatusObjects();
}

void Scene01::destroy()
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

void Scene01::checkStatusObjects()
{
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

void Scene01::handleData(Serializable * object)
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
			this->getPlayer(command->uniqueId)->updateInput(command->input, command->begin);
		}
		break;
	}
	default:
		break;
	}
}