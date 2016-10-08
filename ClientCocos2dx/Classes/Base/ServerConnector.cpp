#include "ServerConnector.h"
#include "Scene\HelloWorldScene.h"
#include "GameObject\Player.h"

ServerConnector::ServerConnector()
{
}

ServerConnector::~ServerConnector()
{
	// close socket
	closesocket(_socket);
	WSACleanup();
}

bool ServerConnector::init(u_short port, char * address)
{
	// start winsocket
	if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
	{
		return false;
	}

	// create socket
	if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		return false;
	}

	// server info
	_server.sin_addr.s_addr = inet_addr(address);
	_server.sin_family = AF_INET;
	_server.sin_port = htons(port);

	_connecting = false;

	_oldPack.direction = 0;
	_oldPack.id = 0;
	_oldPack.x = 0;
	_oldPack.y = 0;
	_oldPack.dx = 0;
	_oldPack.dy = 0;

	_timeVal.tv_sec = 0;
	_timeVal.tv_usec = 0;

	return true;
}

bool ServerConnector::connectServer()
{
	if (connect(_socket, (sockaddr*)&_server, sizeof(_server)) < 0)
	{
		return false;
	}

	// set non-bloking socket
	u_long nonBlocking = 1;
	if (ioctlsocket(_socket, FIONBIO, &nonBlocking) == SOCKET_ERROR)
	{
		return false;
	}

	_connecting = true;

	return true;
}

int counter = 0;

void ServerConnector::recieveData()
{
	DWORD flags = 0;

	CHAR buffer[sizeof(ObjectPacket)];
	WSABUF dataBuffer;
	dataBuffer.buf = buffer;
	dataBuffer.len = sizeof(ObjectPacket);
	DWORD recvBytes;

	//do
	//{
		if (WSARecv(_socket, &dataBuffer, 1, &recvBytes, &flags, NULL, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				// printf("WSARecv() failed with error %d\n", WSAGetLastError());
				this->closeConnection();
			}

			// printf("WSARecv() is OK!\n");
			return;
		}
		else
		{
			// read data buffer with recvBytes
			// ...
			if (recvBytes < dataBuffer.len)
				return;

			ObjectPacket* packet = (ObjectPacket*)dataBuffer.buf;

			// output log
			char str[100];
			sprintf(str, "data %d: (%.2f, %.2f)", counter, packet->x, packet->y);
			counter++;
			log(str);

			// set other position
			HelloWorld::instance->_otherPlayer->setPosition(packet->x, packet->y);
			HelloWorld::instance->_otherPlayer->setDirection((eDirection)packet->direction);
			HelloWorld::instance->_otherPlayer->addStatus(eStatus::RUNNING);
		}
	//} while (recvBytes < dataBuffer.len);
}

void ServerConnector::update(cocos2d::Layer* scene)
{
	if (!_connecting)
		return;

	// select
	FD_ZERO(&_readSet);
	FD_SET(_socket, &_readSet);

	int total = select(0, &_readSet, NULL, NULL, &_timeVal);

	if (total > 0)
	{
		if (FD_ISSET(_socket, &_readSet))
		{
			recieveData();
		}
	}

}

void ServerConnector::closeConnection()
{
	closesocket(_socket);
	_connecting = false;
}

void ServerConnector::sendData(char * buffer)
{
	WSABUF dataBuffer;
	dataBuffer.buf = buffer;
	dataBuffer.len = strlen(buffer);
	DWORD sendBytes;

	if (WSASend(_socket, &dataBuffer, 1, &sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			//printf("WSASend() failed with error %d\n", WSAGetLastError());
			this->closeConnection();
		}

		// printf("WSASend() is OK!\n");
		return;
	}
	else
	{

	}
}

void ServerConnector::sendData(ObjectPacket* packet)
{
	// test
	if (_oldPack.x == packet->x && _oldPack.y == packet->y)
		return; // ko gửi package trùng

	_oldPack.direction = packet->direction;
	_oldPack.id = packet->id;
	_oldPack.x = packet->x;
	_oldPack.y = packet->y;
	_oldPack.dx = packet->dx;
	_oldPack.dy = packet->dy;

	WSABUF dataBuffer;
	dataBuffer.buf = (char*)packet;
	dataBuffer.len = sizeof(ObjectPacket);
	DWORD sendBytes;

	do
	{
		if (WSASend(_socket, &dataBuffer, 1, &sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				//printf("WSASend() failed with error %d\n", WSAGetLastError());
				this->closeConnection();
			}

			// printf("WSASend() is OK!\n");
			return;
		}
		else
		{
			if (sendBytes < dataBuffer.len)
			{
				dataBuffer.buf += sendBytes;
				dataBuffer.len -= sendBytes;
				sendBytes = 0;
			}
		}
	}
	while (sendBytes < dataBuffer.len);
}

u_long ServerConnector::getDataPendingInSocket(SOCKET socket)
{
	u_long pending = 0;

	ioctlsocket(socket, FIONREAD, &pending);

	return pending;
}
