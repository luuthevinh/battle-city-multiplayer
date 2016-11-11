#include "Server.h"
#include "Shared\DataPacket.h"

Server* Server::instance = nullptr;

Server::Server(u_short port, char * address)
{
	_port = port;
	_address = address;

	_timeOut.tv_sec = 0;
	_timeOut.tv_usec = 500;

	_detalTime = 0;
	_lastTime = 0;

}

Server::~Server()
{
}

bool Server::init()
{
	instance = this;

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

	// data handler with queue
	_dataHandler = new DataHandler();

	// factory
	_factory = new ServerConverterFactory(_dataHandler);

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
			continue;
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
				this->recieveData(currentSocket);
			}

			// socket để gửi 
			if (FD_ISSET(currentSocket, &_writeSet))
			{
				// kiểm tra xem list còn client hoặc phải player 0  ko
				if (_clientManager->getAllClients().size() <= 0)
					continue;

				// send package in socket
				this->sendData(currentSocket);
			}
		}

		// game time update
		_gameTime->update();

		_detalTime = _gameTime->getTotalTime() - _lastTime;

		if (_detalTime >= _game->getFrameRate())
		{
			_lastTime += _game->getFrameRate();

			// update object with recieved data
			auto data = _factory->convertNext();
			if (data != nullptr)
			{
				_game->handleData(data);
			}

			// update game
			_game->update(_detalTime);

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

	delete _game;
	delete _clientManager;
	delete _factory;
	delete _dataHandler;
}

void Server::recieveData(SOCKET socket)
{
	if (socket == 0)
		return;

	DWORD flags = 0;
	CHAR buffer[DATA_BUFFER_RECIEVE_SIZE];

	WSABUF dataBuffer;
	dataBuffer.buf = buffer;
	dataBuffer.len = DATA_BUFFER_RECIEVE_SIZE;

	DWORD recvBytes;

	if (WSARecv(socket, &dataBuffer, 1, &recvBytes, &flags, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("recieve data failed with error %d\n", WSAGetLastError());
			this->closeConnection(socket);
		}

		// printf("recieve data is OK but not saved to queue! :(\n");
		return;
	}
	else
	{
		// nhận được thì bỏ vào queue để đọc
		_dataHandler->recieve(dataBuffer.buf, recvBytes);
	}
}

void Server::sendData(SOCKET socket)
{
	if (socket == 0)
		return;

	// có data ko
	if (_dataHandler->getSendQueue(socket) == nullptr || _dataHandler->getSendQueue(socket)->getIndex() <= 0)
		return;

	DWORD sendBytes;
	WSABUF dataBuffer;
	dataBuffer.buf = _dataHandler->getSendQueue(socket)->getData();
	dataBuffer.len = _dataHandler->getSendQueue(socket)->getIndex();
	

	if (WSASend(socket, &dataBuffer, 1, &sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("send failed with error %d\n", WSAGetLastError());
			this->closeConnection(socket);
		}
		else
		{
			//printf("send is OK! socket: %d error: %d\n", socket, WSAGetLastError());
		}

		return;
	}
	else
	{
		// send xong xóa khỏi queue
		_dataHandler->getSendQueue(socket)->popFront(sendBytes);
	}
}

void Server::closeConnection(SOCKET socket)
{
	printf("connection %d closed!\n", socket);

	closesocket(socket);

	_clientManager->removeClient(socket);
	_game->removePlayer(_clientManager->getSocketId(socket));
}

void Server::addConnection(SOCKET socket)
{
	int index = _clientManager->addClient(socket);

	_game->addPlayer(index);

	// reply lại id trong server
	ReplyPacket* rep = new ReplyPacket();
	rep->uniqueId = index;

	_dataHandler->sendTo(socket, rep);

	delete rep;

	printf("new connection: %d\n", socket);
}

DataHandler * Server::getDataHandler()
{
	return _dataHandler;
}

void Server::send(Serializable * object)
{
	for (auto client : _clientManager->getAllClients())
	{
		_dataHandler->sendTo(client, object);
	}
}
