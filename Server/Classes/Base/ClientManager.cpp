#include "ClientManager.h"

int ClientManager::playerId = 10000;

ClientManager::ClientManager()
{
	_roomHasChanged = false;
}

ClientManager::~ClientManager()
{
	for (auto it = _infoPlayers.begin(); it != _infoPlayers.end(); it++)
	{
		if (it->second)
		{
			delete it->second;
		}
	}

	_infoPlayers.clear();
}

int ClientManager::addClient(SOCKET socket)
{
	_clientSockets.push_back(socket);

	auto player = new Player(eObjectId::YELLOW_TANK, _clientSockets.size() - 1);
	player->setUniqueId(this->getNextPlayerId());

	if (_infoPlayers.size() == 0)
		player->setHost(true);

	_infoPlayers[socket] = player;
	
	onChanged();

	return _clientSockets.size() - 1;
}

SOCKET ClientManager::getClientSocket(int id)
{
	if (_clientSockets.size() <= 0 || id >= _clientSockets.size())
		return 0;

	return _clientSockets.at(id);
}

int ClientManager::getSocketId(SOCKET socket)
{
	if (std::find(_clientSockets.begin(), _clientSockets.end(), socket) == _clientSockets.end())
		return -1;

	int count = 0;
	for (auto s : _clientSockets)
	{
		if (s == socket)
		{
			break;
		}
		count++;
	}

	return count;
}

void ClientManager::removeClient(int id)
{
	_clientSockets.erase(_clientSockets.begin() + id);
}

void ClientManager::removeClient(SOCKET socket)
{
	auto result = std::find(_clientSockets.begin(), _clientSockets.end(), socket);
	if (result == _clientSockets.end())
		return;

	if (_infoPlayers.at(socket)->isHost())
	{
		_infoPlayers.begin()->second->setHost(true);
	}

	_infoPlayers.erase(socket);
	_clientSockets.erase(result);

	this->onChanged();
}

const std::vector<SOCKET>& ClientManager::getAllClients()
{
	return _clientSockets;
}

std::vector<Player*> ClientManager::getAllPlayers()
{
	auto ret = std::vector<Player*>();

	for (auto it = _infoPlayers.begin(); it != _infoPlayers.end(); it++)
	{
		ret.push_back(it->second);
	}

	return ret;
}

Player * ClientManager::getPlayerByUniqueId(int id)
{
	for (auto it = _infoPlayers.begin(); it != _infoPlayers.end(); it++)
	{
		if (it->second->getUniqueId() == id)
			return it->second;
	}

	return nullptr;
}

Player * ClientManager::getPlayerBySocket(SOCKET socket)
{
	if(_infoPlayers.find(socket) == _infoPlayers.end())
		return nullptr;

	return _infoPlayers.at(socket);
}

bool ClientManager::hasChanged()
{
	return _roomHasChanged;
}

void ClientManager::onChanged()
{
	_roomHasChanged = true;
}

void ClientManager::setChanged(bool value)
{
	_roomHasChanged = value;
}

int ClientManager::getNextPlayerId()
{
	playerId += 1;
	return playerId;
}
