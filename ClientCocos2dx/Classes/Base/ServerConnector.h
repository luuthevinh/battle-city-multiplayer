#ifndef __SERVERCONNECTOR_H__
#define __SERVERCONNECTOR_H__

#include <WinSock2.h>
#include "cocos2d.h"

#include "Base\Definitions.h"

class ServerConnector
{
public:
	ServerConnector();
	~ServerConnector();

	bool init(u_short port,  char* address);
	bool connectServer();

	void recieveData();

	void update(cocos2d::Layer* scene);

	void closeConnection();
	void sendData(char* buffer);
	void sendData(const Packet &packet);

	u_long getDataPendingInSocket(SOCKET socket);

	int getServerIndex();

	void handlePacket(const Packet &packet);

private:
	WSADATA _wsaData;
	SOCKET _socket;
	fd_set _readSet;
	timeval _timeVal;

	struct sockaddr_in _server;

	bool _connecting;

	int _serverIndex;

	Packet _oldPack;
};


#endif // !__SERVERCONNECTOR_H__
