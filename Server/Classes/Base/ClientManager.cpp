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
	// clear mấy packet đã gửi rồi
	for (auto i = 0; i < _packetInfoQueue.size(); i++)
	{
		if (_packetInfoQueue[i].isSentAll())
		{
			_packetInfoQueue.erase(_packetInfoQueue.begin() + i);
			i--;
		}
	}

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
		{
			auto object = (Tank*)game.getAllGameObjects()[i];
			Packet packet;

			packet.packetType = packet.TANK;
			packet.TankPacket.id = object->getTag();
			packet.TankPacket.direction = object->getDirection();
			packet.TankPacket.status = object->getStatus();
			packet.TankPacket.x = object->getPosition().x;
			packet.TankPacket.x = object->getPosition().y;

			PacketInfo info(packet, this->getAllClients());

			if (_packetInfoQueue.size() == 0)
			{
				_packetInfoQueue.push_back(info);
			}
			else if (!(_packetInfoQueue.back().getInfo() == packet))
			{
				_packetInfoQueue.push_back(info);
			}

			// update packet rồi thì set lại
			game.getAllGameObjects()[i]->setChanged(false);

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

			PacketInfo info(packet, this->getAllClients());

			_packetInfoQueue.push_back(info);

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
	default:
		break;
	}
}

std::deque<PacketInfo>& ClientManager::getPacketInfoQueue()
{
	return _packetInfoQueue;
}

void ClientManager::removeFrontPacket()
{
	if (_packetInfoQueue.size() > 0)
		_packetInfoQueue.erase(_packetInfoQueue.begin());
}

void ClientManager::clearPacketQueue()
{
	while (!_packetInfoQueue.empty())
	{
		_packetInfoQueue.pop_back();
	}
}

void ClientManager::addPacketToQueue(const Packet & packet)
{
	PacketInfo info(packet, this->getAllClients());
	_packetInfoQueue.push_back(info);

	//if (_packetInfoQueue.size() == 0)
	//{
	//	_packetInfoQueue.push_back(info);
	//}
	//else if (!(_packetInfoQueue.back().getInfo() == packet))
	//{
	//	_packetInfoQueue.push_back(info);
	//}
}

void ClientManager::updateFrontPacket(SOCKET socket, bool value)
{
	if (_packetInfoQueue.size() == 0)
		return;

	_packetInfoQueue.front().updateClient(socket, value);
}

