#include "PacketInfo.h"

PacketInfo::PacketInfo(const Packet & info, const std::vector<SOCKET> &toClients)
{
	_info = info;

	switch (info.packetType)
	{
		case Packet::PLAYER:
		{
			//_sentClients[info.PlayerPacket.toSocket] = false;
			break;
		}
		default:
		{
			// cập nhật clients
			for (auto i = 0; i < toClients.size(); i++)
			{
				if (info.fromSocket == toClients[i])
					continue;

				_sentClients[toClients[i]] = false;
			}

			break;
		}
	}
}

PacketInfo::PacketInfo(const Packet & info, SOCKET client)
{
	_sentClients[client] = false;
}

PacketInfo::~PacketInfo()
{
}

const Packet & PacketInfo::getInfo()
{
	return _info;
}

bool PacketInfo::isSentAll()
{
	for (auto i = _sentClients.begin(); i != _sentClients.end(); i++)
	{
		if (i->second == false)
			return false;
	}

	return true;
}

void PacketInfo::updateClient(SOCKET socket, bool value)
{
	if (_sentClients.find(socket) == _sentClients.end())
		return;

	_sentClients[socket] = value;
}

bool PacketInfo::checkSocket(SOCKET socket)
{
	// ko tìm thấy coi như là gửi rồi
	if (_sentClients.find(socket) == _sentClients.end())
		return true;

	return _sentClients[socket];
}
