#include "PlayScene.h"
#include "..\Server.h"
#include "..\GameObject\Wall.h"

#include "..\Shared\DataPacket.h"
#include "..\GameObject\MapLoader.h"
#include "..\GameObject\TankBot.h"
#include "..\Shared\AStarMap.h"
#include "..\Shared\SharedDefinitions.h"
#include "..\GameObject\Eagle.h"
#include "..\Base\SceneManager.h"
#include "..\Scene\OverScene.h"

#include <thread>

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{

}

bool PlayScene::init()
{

	GameObject::setLastUniqueId(0);

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
			auto flag = new Eagle();
			flag->setPosition(Vector2(object->x + object->width / 2, WINDOW_HEIGHT - object->y - object->height / 2));
			this->addStaticObject(flag);
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

	for (auto i = 0; i < _gameObjects.size(); i++)
	{
		_gameObjects[i]->update(dt);
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

	// chuyển scene nếu thua
	if (_isOver)
	{
		printf("game over!\n");

		Server::instance->takeAndSendSnapshot();

		auto over = new IntegerPacket();
		over->integerType = IntegerPacket::GAME_OVER;
		over->value = 1;

		Server::instance->send(over);
		delete over;

		Game::instance->setInGame(false);
		SceneManager::getInstance()->replaceScene(new OverScene());
	}
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

	delete _snapshot;
	delete _aStarMap;
	delete _botSpawner;
}

void PlayScene::checkCollisionObjects(float dt)
{
	std::thread thread1([&] 
	{
		for (auto player : _players)
		{
			for (auto other : _staticObjects)
			{
				player->checkCollision(*other, dt);
			}

			for (auto other : _gameObjects)
			{
				player->checkCollision(*other, dt);
			}
		}
	});

	std::thread thread2([&] 
	{
		for (auto object : _gameObjects)
		{
			for (auto other : _staticObjects)
			{
				object->checkCollision(*other, dt);
			}

			for (auto other : _gameObjects)
			{
				if (other == object)
					continue;

				object->checkCollision(*other, dt);
			}

			for (auto other : _players)
			{
				object->checkCollision(*other, dt);
			}
		}
	});

	for (auto player : _players)
	{
		for (auto other : _players)
		{
			if (other != player)
			{
				player->checkCollision(*other, dt);
			}
		}
	}

	thread1.join();
	thread2.join();
}

void PlayScene::checkStatusObjects()
{
	// xóa object có status là die
	for (auto it = _gameObjects.begin(); it != _gameObjects.end(); )
	{
		if ((*it)->getStatus() == eStatus::DIE)
		{
			if ((*it)->getId() == _botSpawner->getObjectType())
			{
				_botSpawner->setCurrent(_botSpawner->getCurrent() - 1);
			}

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
			if ((*it)->getLife() > 0)
			{
				(*it)->revive();
				(*it)->setPosition(this->getPlayerStartPosition());
				(*it)->onChanged();

				auto revive = new IntegerPacket(IntegerPacket::PLAYER_REVIVE, (*it)->getUniqueId());
				Server::instance->send(revive);
				delete revive;

				continue;
			}
			else
			{
				auto over = new IntegerPacket(IntegerPacket::GAME_OVER, 1);
				Server::instance->send(over);
				delete over;
			}

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
	if (!Game::instance->isInGame())
		return;
	
	_snapshot->addObject(object);
}

void PlayScene::createBot()
{
}

void PlayScene::handleData(Serializable * object)
{
	switch (object->getType())
	{
	case COMMAND:
	{
		// player
		auto player = this->getPlayer(object->getUniqueId());
		if (player != nullptr)
		{
			player->handleData(object);
		}
		break;
	}
	case INTEGER:
	{
		this->handleIntegerPacket((IntegerPacket*)object);
		if (object != nullptr)
			delete object;

		break;
	}
	default:
		break;
	}
}

void PlayScene::handleIntegerPacket(IntegerPacket * integer)
{
	auto integerType = integer->integerType;
	switch (integerType)
	{
	case IntegerPacket::PLAYER_CHARACTER_SELECTION:
	{
		auto player = Server::instance->getClientManager()->getPlayerByUniqueId(integer->getUniqueId());
		if (player)
		{
			player->setId((eObjectId)integer->value);
			Server::instance->getClientManager()->onChanged();
		}
		break;
	}
	case IntegerPacket::BEGIN_PLAY:
	{
		auto playerInfo = Server::instance->getClientManager()->getPlayerByUniqueId(integer->getUniqueId());
		if (playerInfo)
		{
			this->addPlayer(playerInfo);

			auto to = Server::instance->getClientManager()->getClientSocket(playerInfo->getIndex());
			Server::instance->sendTo(to, playerInfo);

			if (playerInfo->isHost())
			{
				this->beginGame();
			}
		}
		break;
	}
	case IntegerPacket::SET_BOT:
	{
		auto player = Server::instance->getClientManager()->getPlayerByUniqueId(integer->getUniqueId());
		if (player && player->isHost())
		{
			this->updateBots(integer->value);
		}
		break;
	}
	default:
		break;
	}
}

void PlayScene::updateBots(int value)
{
	_totalBots += value;

	if (_totalBots < 0)
		_totalBots = MAX_NUMBER_OF_BOTS;

	if (_totalBots > MAX_NUMBER_OF_BOTS)
		_totalBots = 0;

	Server::instance->getClientManager()->onChanged();
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

void PlayScene::addPlayer(Player * player)
{
	auto p = new Player(player->getId(), player->getIndex());
	p->setHost(player->isHost());
	p->setUniqueId(player->getUniqueId());

	Scene::addPlayer(p);
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
	printf("start game!!!\n");
	_botSpawner->setTotalObjects(_totalBots);

	Game::instance->setInGame(true);
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

int PlayScene::getNumberOfBots()
{
	return _totalBots;
}
