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
	object->setTag(GameObject::getNextTag());

	return object->getTag();
}

int Scene::addStaticObject(GameObject * object)
{
	_staticObjects.push_back(object);
	object->setTag(GameObject::getNextTag());

	return object->getTag();
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
	auto player = new Player(eObjectId::YELLOW_TANK, socketIndex);
	player->setTag(GameObject::getNextTag());

	player->setPosition(WINDOW_WIDTH / 2, 64);

	_players.push_back(player);

	return player->getTag();
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