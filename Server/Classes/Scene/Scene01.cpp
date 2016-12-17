#include "Scene01.h"
#include "..\Server.h"
#include "..\GameObject\Wall.h"

#include "..\Shared\DataPacket.h"
#include "..\GameObject\MapLoader.h"

Scene01::Scene01()
{
}

Scene01::~Scene01()
{
	delete _snapshot;
}

bool Scene01::init()
{
	auto loader = MapLoader::createWithTMX("Resources/map/map_01.tmx");
	auto objects = loader->getObjectsInLayer("wall");

	for (auto obj : objects)
	{
		this->addStaticObject(obj);
	}

	_snapshot = new WorldSnapshot();

	return true;
}

void Scene01::update(float dt)
{
	this->checkCollisionObjects(dt);

	// update
	for (auto object : _staticObjects)
	{
		object->update(dt);
	}

	for (auto object : _gameObjects)
	{
		object->update(dt);
	}

	for (auto player : _players)
	{
		player->update(dt);
	}

	_snapshot->setServerTime(_snapshot->getServerTime() + dt);

	this->sendChangedObjects();

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

	while (!_staticObjects.empty())
	{
		delete _staticObjects.back();
		_staticObjects.pop_back();
	}

	while (!_players.empty())
	{
		delete _players.back();
		_players.pop_back();
	}
}

void Scene01::checkCollisionObjects(float dt)
{
	std::vector<GameObject*> mergeList;
	mergeList.insert(mergeList.end(), _gameObjects.begin(), _gameObjects.end());
	mergeList.insert(mergeList.end(), _staticObjects.begin(), _staticObjects.end());

	// check collision
	for (auto player : _players)
	{
		for (auto other : mergeList)
		{
			player->checkCollision(*other, dt);
		}
	}

	for (auto object : _gameObjects)
	{
		for (auto other : mergeList)
		{
			if (other == object)
				continue;

			object->checkCollision(*other, dt);
		}
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

	for (auto it = _staticObjects.begin(); it != _staticObjects.end(); )
	{
		if ((*it)->getStatus() == eStatus::DIE)
		{
			delete *it;
			it = _staticObjects.erase(it);
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

void Scene01::sendChangedObjects()
{
	// gửi object có thay đổi
	for (auto object : _gameObjects)
	{
		if (object->hasChanged())
		{
			this->updateSnapshot(object);
			object->setChanged(false);
		}
	}

	for (auto object : _staticObjects)
	{
		if (object->hasChanged())
		{
			this->updateSnapshot(object);
			object->setChanged(false);
		}
	}

	for (auto player : _players)
	{
		if (player->hasChanged())
		{
			this->updateSnapshot(player);
			player->setChanged(false);
		}
	}
}

void Scene01::updateSnapshot(Serializable * object)
{
	_snapshot->addObject(object);
}

void Scene01::handleData(Serializable * object)
{
	// player
	auto player = this->getPlayer(object->getUniqueId());
	if (player != nullptr)
	{
		player->handleData(object);
	}
}

void Scene01::sendInitDataTo(SOCKET socket)
{
	for (auto object : _gameObjects)
	{
		Server::instance->sendTo(socket, object);
	}

	for (auto object : _staticObjects)
	{
		Server::instance->sendTo(socket, object);
	}

	for (auto player : _players)
	{
		Server::instance->sendTo(socket, player);
	}

	auto reppack = new ReplyPacket();
	reppack->setUniqueId(0);
	reppack->beginTime = -1.0f;

	Server::instance->sendTo(socket, reppack);

	delete reppack;
}