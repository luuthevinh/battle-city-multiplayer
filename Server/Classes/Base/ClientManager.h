#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__

#include <vector>
#include <WinSock2.h>
#include <map>
#include <queue>
#include <deque>

// #include "PacketInfo.h"
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

	const Packet& getPacket(SOCKET socket);

	void removeFrontPacket(SOCKET socket);
	void addPacketToQueue(const Packet& packet, SOCKET);

private:
	std::vector<SOCKET> _clientSockets;

	std::map<SOCKET, std::deque<Packet>> _packetSocketQueue;

};


#endif // !__CLIENTMANAGER_H__
