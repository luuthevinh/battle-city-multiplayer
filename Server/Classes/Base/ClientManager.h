#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__

#include <vector>
#include <WinSock2.h>
#include <map>
#include <queue>
#include <deque>

#include "PacketInfo.h"
#include "../Definitions.h"
#include "../Game.h"

class ClientManager
{
public:
	ClientManager();
	~ClientManager();

	int addClient(SOCKET socket);
	SOCKET getClientSocket(int id);
	void removeClient(int id);

	const std::vector<SOCKET> &getAllClients();

	void generatePackets(const Game &game);
	void updatePacket(Game &game, const Packet& packet, int from);

	std::deque<PacketInfo> &getPacketInfoQueue();

	void removeFrontPacket();
	void clearPacketQueue();
	void addPacketToQueue(const Packet& packet);

	void updateFrontPacket(SOCKET socket, bool value);

private:
	std::vector<SOCKET> _clientSockets;

	std::deque<PacketInfo> _packetInfoQueue;
};


#endif // !__CLIENTMANAGER_H__
