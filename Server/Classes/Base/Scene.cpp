#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::handleData(Serializable * object)
{
}

int Scene::addObject(GameObject * object)
{
	_gameObjects.push_back(object);
	object->setUniqueId(GameObject::getNextUniqueId());

	return object->getUniqueId();
}

int Scene::addStaticObject(GameObject * object)
{
	_staticObjects.push_back(object);

	object->setUniqueId(GameObject::getNextUniqueId());
	object->setParentScene(this);

	return object->getUniqueId();
}

void Scene::removeObject(int tag)
{
	for (auto it = _gameObjects.begin(); it != _gameObjects.end(); it++)
	{
		if ((*it)->getUniqueId() == tag)
		{
			delete *it;
			_gameObjects.erase(it);
			return;
		}
	}
}

int Scene::addPlayer(int socketIndex)
{
	auto player = new Player(eObjectId::YELLOW_TANK, socketIndex);
	player->setUniqueId(GameObject::getNextUniqueId());

	player->setPosition(WINDOW_WIDTH / 2, 64);

	_players.push_back(player);

	return player->getUniqueId();
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
		if (player->getUniqueId() == tag)
			return player;
	}
	
	return nullptr;
}

void Scene::sendInitDataTo(SOCKET socket)
{
}

WorldSnapshot * Scene::getSnapshot()
{
	return _snapshot;
}
