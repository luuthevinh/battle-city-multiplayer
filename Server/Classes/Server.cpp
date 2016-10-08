#include "Server.h"

Server::Server(u_short port, char * address)
{
	_port = port;
	_address = address;

	_deltaTime.tv_sec = 0;
	_deltaTime.tv_usec = 16666;
}

Server::~Server()
{
	delete _game;
	delete _clientManager;
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
		if ((total = select(0, &_readSet, &_writeSet, NULL, &_deltaTime)) == SOCKET_ERROR)
		{
			return;
		}

		// update game, tạm để 0.33
		_game->update(0.33f);

		if (total == 0)
			continue;

		// kiểm tra kết nối tới server
		if (FD_ISSET(_listenSocket, &_readSet))
		{
			total--;

			SOCKET acceptSocket;

			if ((acceptSocket = accept(_listenSocket, NULL, NULL)) != INVALID_SOCKET)
			{
				// set socket kết nối tới server thành non-blocking
				_nonBlocking = 1;
				if (ioctlsocket(acceptSocket, FIONBIO, &_nonBlocking) == SOCKET_ERROR)
				{
					printf("ioctlsocket(FIONBIO) failed!\n");
					return;
				}

				// lưu lại socket
				addConnection(acceptSocket);
			}
			else
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					printf("accept() failed");
					return;
				}

				printf("accept() is fine!\n");
			}
		}

		// xử lý kết nối
		for (int i = 0; i < _clientManager->getAllClients().size(); i++)
		{
			SOCKET currentSocket = _clientManager->getClientSocket(i);

			// socket để đọc
			if (FD_ISSET(currentSocket, &_readSet) && currentSocket == _clientManager->getClientSocket(0))
			{
				// đọc tin
				this->recievePackage(i);
			}

			// socket để gửi 
			if (FD_ISSET(currentSocket, &_writeSet))
			{
				// kiểm tra xem list còn client hoặc phải player 0  ko
				if (_clientManager->getAllClients().size() <= 0 || currentSocket == _clientManager->getClientSocket(0))
					continue;

				// send package in socket
				this->sendPackage(i);
			}
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

	DWORD flags = 0;

	CHAR buffer[sizeof(ObjectPacket)];
	WSABUF dataBuffer;
	dataBuffer.buf = buffer;
	dataBuffer.len = sizeof(ObjectPacket);
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

		ObjectPacket* packet = (ObjectPacket*)dataBuffer.buf;
		printf("data recieved socket %d(%d): (%.2f, %.2f)\n", currentSocket, recvBytes, packet->x, packet->y);
		
		// test cập nhật vị tri player 0
		_game->getPlayer(0)->setPosition(packet->x, packet->y);

		if (packet->direction > 0 && packet->direction <= 4)
		{
			_game->getPlayer(0)->setDirection((eDirection)packet->direction);
			//_game->getPlayer(0)->setVelocity(TANK_NORMAL_VELOCITY);
			//_game->getPlayer(0)->setPosition(packet->x, packet->y);
		}
		else
		{
			//_game->getPlayer(0)->setVelocity(0);
		}

		// tạm để ở đây vì game cập nhật thằng player 0
		for (size_t i = 0; i < _clientManager->getAllClients().size(); i++)
		{
			_clientManager->setUpdate(i, false);
		}


		_game->update(dataBuffer.buf);
	}
}

void Server::sendPackage(int index)
{
	// test
	if (_clientManager->isSendNewUpdate(index) &&
		(_currentPack.x == _game->getPlayer(0)->getPositionX() && _currentPack.y == _game->getPlayer(0)->getPositionY()))
	{
		return;
	}

	SOCKET currentSocket = _clientManager->getClientSocket(index);

	ObjectPacket pack;
	pack.id = 0;
	pack.direction = _game->getPlayer(0)->getDirection();
	pack.x = _game->getPlayer(0)->getPositionX();
	pack.y = _game->getPlayer(0)->getPositionY();
	pack.dx = 0;
	pack.dy = 0;

	_currentPack = pack;
	_clientManager->setUpdate(index, true);

	WSABUF dataBuffer;
	dataBuffer.buf = (CHAR*)(&pack);
	dataBuffer.len = sizeof(ObjectPacket);
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
			printf("Send to %d: (%.2f, %.2f)\n", currentSocket, pack.x, pack.y);
			if (sendBytes < dataBuffer.len)
			{
				dataBuffer.buf += sendBytes;
				dataBuffer.len -= sendBytes;
			}
		}
	} while (sendBytes < dataBuffer.len);
}

void Server::closeConnection(int index)
{
	printf("connection %d closed!", _clientManager->getClientSocket(index));

	closesocket(_clientManager->getClientSocket(index));
	_clientManager->removeClient(index);
}

void Server::addConnection(SOCKET socket)
{
	_game->addPlayer(new Player());

	_clientManager->addClient(socket);

	printf("new connection: %d\n", socket);
}
