#ifndef __SERVERCONNECTOR_H__
#define __SERVERCONNECTOR_H__

#include <WinSock2.h>
#include "cocos2d.h"
#include "Base\Definitions.h"

class Buffer;
class DataHandler;
class ConverterFactory;
class Serializable;

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
	void sendData(SOCKET socket);

	void send(Serializable* object);

	u_long getDataPendingInSocket(SOCKET socket);

	int getServerIndex();
	void handleData();

private:
	WSADATA _wsaData;
	SOCKET _socket;
	fd_set _readSet;
	fd_set _writeSet;
	timeval _timeVal;

	struct sockaddr_in _server;

	bool _connecting;

	int _serverIndex;

	DataHandler* _dataHandler;
	ConverterFactory* _factory;
};


#endif // !__SERVERCONNECTOR_H__
