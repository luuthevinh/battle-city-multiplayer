#include "ClientManager.h"

ClientManager::ClientManager()
{
}

ClientManager::~ClientManager()
{
}

void ClientManager::addClient(SOCKET socket)
{
	_clientSockets.push_back(socket);
	_updatedClients.push_back(false);
}

SOCKET ClientManager::getClientSocket(int id)
{
	return _clientSockets.at(id);
}

void ClientManager::removeClient(int id)
{
	_clientSockets.erase(_clientSockets.begin() + id);
	_updatedClients.erase(_updatedClients.begin() + id);
}

const std::vector<SOCKET>& ClientManager::getAllClients()
{
	return _clientSockets;
}

bool ClientManager::isSendNewUpdate(int id)
{
	return _updatedClients[id];
}

void ClientManager::setUpdate(int id, bool val)
{
	_updatedClients[id] = val;
}
