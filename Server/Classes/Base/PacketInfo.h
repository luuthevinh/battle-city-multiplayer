#ifndef __PACKET_INFO_H__
#define __PACKET_INFO_H__

#include <map>
#include <vector>
#include "../Definitions.h"

class PacketInfo
{
public:
	PacketInfo(const Packet &info, const std::vector<SOCKET> &toClients);
	PacketInfo(const Packet &info, SOCKET client);
	~PacketInfo();

	const Packet &getInfo();

	bool checkSocket(SOCKET socket);
	bool isSentAll();

	void updateClient(SOCKET socket, bool value);

private:
	std::map<SOCKET, bool> _sentClients;

	Packet _info;
};

#endif // !__PACKET_INFO_H__
