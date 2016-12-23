#ifndef __SERVER_H__
#define __SERVER_H__

#include <WinSock2.h>
#include <stdio.h>
#include <vector>

#include "Game.h"
#include "Base\ClientManager.h"
#include "Base\GameTime.h"
#include "Base\ServerConverterFactory.h"
#include "Shared\DataHandler.h"

class Server
{
public:
	static Server* instance;

	Server(u_short port, char* address);
	~Server();

	bool init();
	void run();
	void destroy();

	void recieveData(SOCKET socket);
	void sendDataToSocket(SOCKET socket);
	void closeConnection(SOCKET socket);
	void addConnection(SOCKET socket);

	DataHandler* getDataHandler();
	ClientManager* getClientManager();

	void send(Serializable* object);
	void sendTo(SOCKET socket, Serializable* object);

	const GameTime& getTime();
private:
	u_short _port;
	char* _address;

	bool _running;
	bool _isLocalhost;

	SOCKET _listenSocket;
	WSADATA _wsaData;
	SOCKADDR_IN _internetAddress;
	ULONG _nonBlocking;

	FD_SET _writeSet;
	FD_SET _readSet;

	timeval _timeOut;

	Game* _game;
	ClientManager* _clientManager;

	float _timeStep;
	float _lastTime;
	GameTime* _serverTime;

	DataHandler* _dataHandler;

	int selectSockets();
	void acceptAndAddConnection();
	SOCKET acceptSocket();

	void recieveDataFromAll();
	void sendDataToAllWithTimeStep();

	void sendRoomInfoToAllClients();
	void takeAndSendSnapshot();

	char* _localIp;
	void getLocalIp();
};

#endif // !__SERVER_H__
