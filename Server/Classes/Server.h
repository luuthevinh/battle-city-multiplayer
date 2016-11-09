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
	void sendData(SOCKET socket);

	void closeConnection(SOCKET socket);
	void addConnection(SOCKET socket);

	DataHandler* getDataHandler();

	void send(Serializable* object);

private:
	u_short _port;
	char* _address;

	bool _running;

	SOCKET _listenSocket;
	WSADATA _wsaData;
	SOCKADDR_IN _internetAddress;
	ULONG _nonBlocking;

	FD_SET _writeSet;
	FD_SET _readSet;

	timeval _timeOut;

	Game* _game;
	ClientManager* _clientManager;

	GameTime* _gameTime;
	float _detalTime;
	float _lastTime;

	DataHandler* _dataHandler;
	ServerConverterFactory* _factory;
};

#endif // !__SERVER_H__
