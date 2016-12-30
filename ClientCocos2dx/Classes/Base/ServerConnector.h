#ifndef __SERVERCONNECTOR_H__
#define __SERVERCONNECTOR_H__

#include <WinSock2.h>
#include "cocos2d.h"
#include "Base\Definitions.h"

class Buffer;
class DataHandler;
class ClientConverterFactory;
class Serializable;

class ServerConnector
{
public:
	static ServerConnector* getInstance();

	ServerConnector();
	~ServerConnector();

	bool init(u_short port, const char* address);
	bool connectServer();
	bool isConnected();

	void recieveData();

	void update(float dt);

	void closeConnection();
	void sendData(SOCKET socket);

	void send(Serializable* object);

	u_long getDataPendingInSocket(SOCKET socket);

	int getServerIndex();

	void setTime(float time);
	float getTime();

	float getClientTime();

	void setRun(bool value);
	bool isRunning();

	ClientConverterFactory* getFactory();

	void setUniqueId(int id);
	int getUniqueId();

	void setHost(bool value);
	bool isHost();

private:
	static ServerConnector* _instance;
	static int _lastObjectIdOnServer;

	WSADATA _wsaData;
	SOCKET _socket;
	fd_set _readSet;
	fd_set _writeSet;
	timeval _timeVal;

	struct sockaddr_in _server;

	bool _connecting;

	int _serverIndex;

	DataHandler* _dataHandler;
	ClientConverterFactory* _factory;

	float _timer;
	float _clientTime;
	bool _isRunning;

	int _uniqueIdOnServer;
	bool _isHost;

	void startNewThread();
	void run();
};


#endif // !__SERVERCONNECTOR_H__
