#ifndef __SERVER_H__
#define __SERVER_H__

#include <WinSock2.h>
#include <stdio.h>
#include <vector>

#include "Game.h"
#include "Base\ClientManager.h"
#include "Base\GameTime.h"
#include "Base\PacketHandler.h"

class Server
{
public:
	Server(u_short port, char* address);
	~Server();

	bool init();
	void run();
	void destroy();

	bool recieveMessage(int connectionId);
	bool sendMessage(int connectionId, char* message);

	void recievePackage(int index);
	void sendPackage(int index);

	void closeConnection(int index);
	void addConnection(SOCKET socket);

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

	//Packet _oldPacket;

	GameTime* _gameTime;
	float _detalTime;
	float _lastTime;

	// PacketHandler* _packetHandler;
};

#endif // !__SERVER_H__
