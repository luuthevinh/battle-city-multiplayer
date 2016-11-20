#include "Scene.h"

Scene::Scene()
{
	_uniqueIdCounter = 0;
}

Scene::~Scene()
{
}

void Scene::handleData(Serializable * object)
{
}

int Scene::addObject(GameObject * object)
{
	_uniqueIdCounter++;

	_gameObjects.push_back(object);
	object->setTag(_uniqueIdCounter);

	return _uniqueIdCounter;
}

void Scene::removeObject(int tag)
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

int Scene::addPlayer(int socketIndex)
{
	_uniqueIdCounter++;

	auto player = new Player(eObjectId::YELLOW_TANK, socketIndex);
	player->setTag(_uniqueIdCounter);
	player->setPosition(WINDOW_WIDTH / 2, 64);

	_players.push_back(player);

	return _uniqueIdCounter;
}

void Scene::removePlayer(int index)
{
	for (auto i = 0; i < _players.size(); i++)
	{
		if (_players[i]->getIndex() == index)
		{
			_players.erase(_players.begin() + i);
		}
	}
}

Player * Scene::getPlayer(int tag)
{
	for (auto player : _players)
	{
		if (player->getTag() == tag)
			return player;
	}
	
	return nullptr;
}

void Scene::sendInitDataTo(SOCKET socket)
{
}