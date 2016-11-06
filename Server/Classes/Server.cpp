#include "Server.h"

Server::Server(u_short port, char * address)
{
	_port = port;
	_address = address;

	_timeOut.tv_sec = 0;
	_timeOut.tv_usec = 500;

	_detalTime = 0;
	_lastTime = 0;

	_serverTick = 20;
	_serverLastTime = 0.0f;
	_serverDetal = 1.0f / _serverTick;
}

Server::~Server()
{
	delete _game;
	delete _clientManager;
	//delete _packetHandler;
}

bool Server::init()
{
	// start winsock
	if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
	{
		printf("WSAStartup() error!\n");
		return false;
	}

	// create socket
	if ((_listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("WSASocket() error!\n");
		return false;
	}

	// socket address
	_internetAddress.sin_family = AF_INET;
	_internetAddress.sin_addr.s_addr = inet_addr(_address);
	_internetAddress.sin_port = htons(_port);

	// bind
	if (bind(_listenSocket, (sockaddr*)&_internetAddress, sizeof(_internetAddress)) == SOCKET_ERROR)
	{
		printf("Could not bind ListenSocket!\n");
		return false;
	}

	// listen
	if (listen(_listenSocket, 5))
	{
		printf("Listen error\n");
		return false;
	}

	// đổi từ blocking socket thành non-blocking
	_nonBlocking = 1;
	if (ioctlsocket(_listenSocket, FIONBIO, &_nonBlocking))
	{
		printf("ioctlsocket() failed\n");
		return false;
	}

	// create main game
	_game = new Game();
	_game->init();

	_clientManager = new ClientManager();

	// game time
	_gameTime = new GameTime();
	_gameTime->init();

	//
	//_packetHandler = new PacketHandler();

	return true;
}

void Server::run()
{
	_running = true;

	while (_running)
	{
		// clear read and write set
		FD_ZERO(&_readSet);
		FD_ZERO(&_writeSet);

		// add listen socket into readset
		FD_SET(_listenSocket, &_readSet);

		// set socket để select
		for (auto i = 0; i < _clientManager->getAllClients().size(); i++)
		{
			FD_SET(_clientManager->getClientSocket(i), &_readSet);
			FD_SET(_clientManager->getClientSocket(i), &_writeSet);
		}

		int total = 0;

		// select sockets
		if ((total = select(0, &_readSet, &_writeSet, NULL, &_timeOut)) == SOCKET_ERROR)
		{
			printf("select() error!\n");
			system("pause");
		}

		// kiểm tra kết nối tới server
		if (FD_ISSET(_listenSocket, &_readSet))
		{
			SOCKET acceptSocket;

			// accept socket mới
			if ((acceptSocket = accept(_listenSocket, NULL, NULL)) != INVALID_SOCKET)
			{
				// set socket kết nối tới server thành non-blocking
				_nonBlocking = 1;
				if (ioctlsocket(acceptSocket, FIONBIO, &_nonBlocking) == SOCKET_ERROR)
				{
					printf("ioctlsocket(FIONBIO) failed!\n");
					system("pause");
				}

				// lưu lại socket
				addConnection(acceptSocket);
			}
			else
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					printf("accept() failed");
					system("pause");;
				}

				printf("accept() is fine!\n");
			}
		}

		// xử lý kết nối
		for (int i = 0; i < _clientManager->getAllClients().size(); i++)
		{
			SOCKET currentSocket = _clientManager->getClientSocket(i);

			// socket để đọc
			if (FD_ISSET(currentSocket, &_readSet))
			{
				// đọc tin
				this->recievePackage(i);
			}

			//float delta = _gameTime->getTotalTime() - _serverLastTime;
			//if (delta >= _serverDetal)
			//{
				//_serverLastTime += _serverDetal;

				// socket để gửi 
				if (FD_ISSET(currentSocket, &_writeSet))
				{
					// kiểm tra xem list còn client hoặc phải player 0  ko
					if (_clientManager->getAllClients().size() <= 0)
						continue;

					// send package in socket
					this->sendPackage(i);
				}
			//}
		}

		// game time update
		_gameTime->update();

		_detalTime = _gameTime->getTotalTime() - _lastTime;

		if (_detalTime >= _game->getFrameRate())
		{
			_lastTime += _game->getFrameRate();

			// update game
			_game->update(_detalTime);

			// update packet to send to client
			_clientManager->generatePackets(*_game);

			// title console
			float f = 1.0f / _detalTime;
			char buffer[100];
			sprintf(buffer, "(%.1f/s) | %.4f | Tank Server \n", f, _detalTime); 
			SetConsoleTitle(buffer);
		}
		else
		{
			Sleep((_game->getFrameRate() - _detalTime) * 1000.0f);
		}
	}
}

void Server::destroy()
{
	closesocket(_listenSocket);
	WSACleanup();
}

bool Server::recieveMessage(int connectionId)
{
	return true;
}

bool Server::sendMessage(int connectionId, char * message)
{
	return true;
}

void Server::recievePackage(int index)
{
	SOCKET currentSocket = _clientManager->getClientSocket(index);
	if (currentSocket == 0)
		return;

	DWORD flags = 0;

	CHAR buffer[sizeof(Packet)];
	WSABUF dataBuffer;
	dataBuffer.buf = buffer;
	dataBuffer.len = sizeof(Packet);
	DWORD recvBytes;

	if (WSARecv(currentSocket, &dataBuffer, 1, &recvBytes, &flags, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("WSARecv() failed with error %d\n", WSAGetLastError());

			this->closeConnection(index);
		}

		printf("WSARecv() is OK!\n");

		return;
	}
	else
	{
		// read data buffer with recvBytes
		// ...
		//dataBuffer.buf[recvBytes] = '\0';
		//printf("data recived socket %d: %s\n", currentSocket, dataBuffer.buf);
		//printf("data recieved socket %d(%d): (%.2f, %.2f)\n", currentSocket, recvBytes, packet->x, packet->y);
		
		Packet* packet = (Packet*)dataBuffer.buf;
		_clientManager->updatePacket(*_game, *packet, index);

		//_game->update(dataBuffer.buf);

		// thằng này gửi thì ko gửi lại cho nó
		// _clientManager->setUpdate(index, true);
	}
}

void Server::sendPackage(int index)
{
	// lấy socket
	SOCKET currentSocket = _clientManager->getClientSocket(index);

	if (currentSocket == 0)
		return;

	// packet của socket này
	Packet p = _clientManager->getPacket(currentSocket);

	if (p.packetType == Packet::eType::EMPTY)
		return;

	WSABUF dataBuffer;
	dataBuffer.buf = (CHAR*)(&p);
	dataBuffer.len = sizeof(Packet);
	DWORD sendBytes;

	do
	{
		if (WSASend(currentSocket, &dataBuffer, 1, &sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("WSASend() failed with error %d\n", WSAGetLastError());
				this->closeConnection(index);
			}
			else
				printf("WSASend() is OK! socket: %d error: %d\n", currentSocket, WSAGetLastError());

			return;
		}
		else
		{
			//printf("Send to %d: (%.2f, %.2f)\n", currentSocket, pack.x, pack.y);
			if (sendBytes < dataBuffer.len)
			{
				dataBuffer.buf += sendBytes;
				dataBuffer.len -= sendBytes;
			}
		}
	} while (sendBytes < dataBuffer.len);

	// gửi xong thì xóa
	_clientManager->removeFrontPacket(currentSocket);
}

void Server::closeConnection(int index)
{
	printf("connection %d closed!", _clientManager->getClientSocket(index));

	closesocket(_clientManager->getClientSocket(index));
	
	_clientManager->removeClient(index);
	_game->removePlayer(index);
}

void Server::addConnection(SOCKET socket)
{
	int index = _clientManager->addClient(socket);

	_game->addPlayer(index);

	Packet repPack;
	repPack.packetType = Packet::PLAYER;
	repPack.PlayerPacket.uniqueId = index;

	_clientManager->addPacketToQueue(repPack, socket);

	//Packet packet;
	//packet.fromSocket = socket;
	//packet.packetType = Packet::CREATE;
	//packet.CreatePacket.objectId = eObjectId::YELLOW_TANK;
	//packet.CreatePacket.uniqueId = _game->getPlayer(index)->getTag();
	//packet.CreatePacket.x = 0;
	//packet.CreatePacket.y = 0;

	//_clientManager->addPacketToQueue(packet, socket);

	printf("new connection: %d\n", socket);
}
