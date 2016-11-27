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
	static ServerConnector* getInstance();

	ServerConnector();
	~ServerConnector();

	bool init(u_short port,  char* address);
	bool connectServer();

	void recieveData();

	void update(float dt);

	void closeConnection();
	void sendData(SOCKET socket);

	void send(Serializable* object);

	u_long getDataPendingInSocket(SOCKET socket);

	int getServerIndex();
	void handleData(cocos2d::Layer* scene);

	void setTime(float time);
	float getTime();

	void setRun(bool value);
	bool isRunning();

	ConverterFactory* getFactory();


private:
	static ServerConnector* _instance;

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

	float _timer;
	bool _isRunning;
};


#endif // !__SERVERCONNECTOR_H__
