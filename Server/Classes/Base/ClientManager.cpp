#include "ClientManager.h"

ClientManager::ClientManager()
{
	
}

ClientManager::~ClientManager()
{
}

int ClientManager::addClient(SOCKET socket)
{
	_clientSockets.push_back(socket);

	auto player = new Player(eObjectId::YELLOW_TANK, _clientSockets.size() - 1);
	_infoPlayers[socket] = player;

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

	_clientSockets.erase(result);
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
