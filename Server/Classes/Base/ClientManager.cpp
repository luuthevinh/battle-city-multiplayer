#include "ClientManager.h"
#include "GameObject.h"

ClientManager::ClientManager()
{
	
}

ClientManager::~ClientManager()
{
}

int ClientManager::addClient(SOCKET socket)
{
	_clientSockets.push_back(socket);

	return _clientSockets.size() - 1;
}

SOCKET ClientManager::getClientSocket(int id)
{
	if (_clientSockets.size() <= 0 || id >= _clientSockets.size())
		return 0;

	return _clientSockets.at(id);
}

void ClientManager::removeClient(int id)
{
	_clientSockets.erase(_clientSockets.begin() + id);
}

const std::vector<SOCKET>& ClientManager::getAllClients()
{
	return _clientSockets;
}

void ClientManager::generatePackets(const Game & game)
{
	for (size_t i = 0; i < game.getAllGameObjects().size(); i++)
	{
		// nếu object này ko có thay đổi thì thôi
		if (!game.getAllGameObjects()[i]->hasChanged())
			continue;

		switch (game.getAllGameObjects()[i]->getId())
		{
		case eObjectId::WHITE_TANK:
		case eObjectId::GREEN_TANK:
		case eObjectId::YELLOW_TANK:
		case eObjectId::BULLET:
		{
			auto object = game.getAllGameObjects()[i];
			
			Packet packet = object->getPacket();

			// add packet vào các socket sẽ gửi
			for (size_t i = 0; i < _clientSockets.size(); i++)
			{
				this->addPacketToQueue(packet, _clientSockets[i]);
			}

			// update packet rồi thì set lại
			object->setChanged(false);

			break;
		}
		default:
			break;
		}
	}


	for (size_t i = 0; i < game.getAllPlayers().size(); i++)
	{
		auto player = (Player*)game.getAllPlayers()[i];

		if (!player->hasChanged())
			continue;

		switch (player->getId())
		{
		case eObjectId::WHITE_TANK:
		case eObjectId::GREEN_TANK:
		case eObjectId::YELLOW_TANK:
		{
			if (player->getIndex() >= _clientSockets.size())
				return;

			Packet packet = player->getPacket();
			packet.fromSocket = this->getClientSocket(player->getIndex());

			// add packet vào các socket sẽ gửi
			for (size_t i = 0; i < _clientSockets.size(); i++)
			{
				this->addPacketToQueue(packet, _clientSockets[i]);
			}

			// update packet rồi thì set lại
			player->setChanged(false);

			break;
		}
		default:
			break;
		}
	}
}

void ClientManager::updatePacket(Game &game, const Packet& packet, int from)
{
	switch ((Packet::eType)packet.packetType)
	{
	case Packet::TANK:
	{
		auto player = game.getPlayer(from);
		if (player != nullptr)
		{
			player->setDirection((eDirection)packet.TankPacket.direction);
			player->setStatus((eStatus)packet.TankPacket.status);
		}

		break;
	}
	case Packet::OBJECT:
	{
		break;
	}
	case Packet::PLAYER:
	{
		// nhận input player
		game.handlePlayerInput(packet.PlayerPacket.uniqueId, (eKeyInput)packet.PlayerPacket.playerInput, packet.PlayerPacket.start);
		break;
	}
	default:
		break;
	}
}

const Packet & ClientManager::getPacket(SOCKET socket)
{
	Packet ret;
	ret.packetType = Packet::eType::EMPTY;

	if (_packetSocketQueue.find(socket) == _packetSocketQueue.end())
		return ret;

	auto queue = _packetSocketQueue.at(socket);
	if (queue.size() == 0)
		return ret;

	return _packetSocketQueue.at(socket).front();
}

void ClientManager::removeFrontPacket(SOCKET socket)
{
	if (_packetSocketQueue.find(socket) == _packetSocketQueue.end())
		return;

	_packetSocketQueue.at(socket).pop_front();

	if (_packetSocketQueue.at(socket).size() == 0)
		_packetSocketQueue.erase(socket);
}

void ClientManager::addPacketToQueue(const Packet & packet, SOCKET socket)
{
	_packetSocketQueue[socket].push_back(packet);
}

