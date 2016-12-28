#include "PlayScene.h"
#include "..\Server.h"
#include "..\GameObject\Wall.h"

#include "..\Shared\DataPacket.h"
#include "..\GameObject\MapLoader.h"
#include "..\GameObject\TankBot.h"
#include "..\Shared\AStarMap.h"
#include "..\Shared\SharedDefinitions.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
	delete _snapshot;
	delete _aStarMap;
	delete _botSpawner;
}

bool PlayScene::init()
{
	auto loader = MapLoader::createWithTMX("Resources/map/map_01.tmx");
	if (loader == nullptr)
	{
		printf("cannot create loader tmx.\n");
		system("pause");
	}

	auto objects = loader->getObjectsInLayer("wall");

	int rows = loader->getRows();
	int cols = loader->getColumns();

	_aStarMap = new tank::AStarMap(rows, cols);

	auto layer = loader->getLayer("wall");
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			tank::Point index;
			index.x = j;
			index.y = i;

			_aStarMap->setValue(index, layer->content.gids[i * cols + j]);
		}
	}

	for (auto obj : objects)
	{
		this->addStaticObject(obj);
	}

	// load objects group
	auto objectsLayer = loader->getLayer("objects");
	
	auto object = objectsLayer->content.objgr->head;
	while (object)
	{
		if (strcmp(object->name, "player_01") == 0 || strcmp(object->name, "player_02") == 0 || 
			strcmp(object->name, "player_03") == 0 || strcmp(object->name, "player_04") == 0)
		{
			_beginPlayerPositions.push_back(Vector2(object->x + object->width / 2, WINDOW_HEIGHT - object->y - object->height / 2));
		}
		else if (strcmp(object->name, "enemy_01") == 0 || strcmp(object->name, "enemy_02") == 0 || 
			strcmp(object->name, "enemy_03") == 0)
		{
			_beginBotPositions.push_back(Vector2(object->x + object->width / 2, WINDOW_HEIGHT - object->y - object->height / 2));
		}
		else if (strcmp(object->name, "flag") == 0)
		{

		}

		object = object->next;
	}

	_snapshot = new WorldSnapshot();

	_numberOfBotsCreated = 0;

	_botSpawner = new Spawner(eObjectId::WHITE_TANK, this);
	_botSpawner->setTime(5.0f);

	return true;
}

void PlayScene::update(float dt)
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

	_botSpawner->update(dt);
}

void PlayScene::destroy()
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

void PlayScene::checkCollisionObjects(float dt)
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

void PlayScene::checkStatusObjects()
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

void PlayScene::sendChangedObjects()
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

void PlayScene::updateSnapshot(Serializable * object)
{
	_snapshot->addObject(object);
}

void PlayScene::createBot()
{
}

void PlayScene::handleData(Serializable * object)
{
	// player
	auto player = this->getPlayer(object->getUniqueId());
	if (player != nullptr)
	{
		player->handleData(object);
	}
}

void PlayScene::sendInitDataTo(SOCKET socket)
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

int PlayScene::addPlayer(int socketIndex)
{
	return Scene::addPlayer(socketIndex);;
}

void PlayScene::updateMap(const tank::Point& index, int value)
{
	if (_aStarMap == nullptr)
		return;

	_aStarMap->setValue(index, value);
}

void PlayScene::updateMap(const Vector2 & position, int value)
{
	auto index = _aStarMap->positionToIndex(tank::Point(position.x, position.y));
	this->updateMap(index, value);
}

tank::AStarMap * PlayScene::getMap()
{
	return _aStarMap;
}

void PlayScene::beginGame()
{
	_botSpawner->setTotalObjects(Game::instance->getNumberOfBots());
}

int PlayScene::getRandomPositionIndex()
{
	return rand() % _beginBotPositions.size();
}

Vector2 PlayScene::getBotStartPosition(int index)
{
	return _beginBotPositions[index];
}

const Vector2 & PlayScene::getPlayerStartPosition()
{
	auto randIndex = rand() % _beginPlayerPositions.size();

	return _beginPlayerPositions[randIndex];
}
