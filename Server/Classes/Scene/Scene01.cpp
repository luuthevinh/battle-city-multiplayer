#include "Scene01.h"
#include "..\Server.h"
#include "..\GameObject\Wall.h"

#include "..\Shared\DataPacket.h"

Scene01::Scene01()
{
}

Scene01::~Scene01()
{
}

bool Scene01::init()
{
	// demo wall
	for (size_t r = 0; r < 8; r++)
	{
		if (r % 2 != 0)
			continue;

		for (size_t c = 0; c < 14; c++)
		{
			if (c % 2 != 0)
				continue;

			auto wall = Wall::createWithPosition(eObjectId::BRICK_WALL, Vector2(64 + (32 * c), 256 + (32 * r)));
			this->addObject(wall);
		}

	}

	return true;
}

void Scene01::update(float dt)
{
	// update and check collision
	for (auto object : _gameObjects)
	{
		for (auto p : _players)
		{
			object->checkCollision(*p, dt);
		}

		for (auto other : _gameObjects)
		{
			if (other == object)
				continue;

			object->checkCollision(*other, dt);
		}

		object->update(dt);
	}

	// gửi object có thay đổi
	for (auto object : _gameObjects)
	{
		if (object->hasChanged())
		{
			Server::instance->send(object);
			object->setChanged(false);
		}
	}

	for (auto player : _players)
	{
		for (auto other : _players)
		{
			if (other != player)
			{
				player->checkCollision(*other, dt);
			}
		}

		for (auto object : _gameObjects)
		{
			player->checkCollision(*object, dt);
		}

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

	for (auto it = _players.begin(); it != _players.end(); )
	{
		if ((*it)->getStatus() == eStatus::DIE)
		{
			delete *it;
			it = _players.erase(it);
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

void Scene01::sendInitDataTo(SOCKET socket)
{
	for (auto object : _gameObjects)
	{
		Server::instance->sendTo(socket, object);
	}

	for (auto player : _players)
	{
		Server::instance->sendTo(socket, player);
	}
}