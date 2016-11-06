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

	_oldPack.TankPacket.direction = 0;
	_oldPack.TankPacket.id = 0;
	_oldPack.TankPacket.x = 0;
	_oldPack.TankPacket.y = 0;

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

	CHAR buffer[sizeof(Packet)];
	WSABUF dataBuffer;
	dataBuffer.buf = buffer;
	dataBuffer.len = sizeof(Packet);
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

			Packet* packet = (Packet*)dataBuffer.buf;

			this->handlePacket(*packet);

			//if (packet->packetType == (uint8_t)Packet::TANK)
			//{
			//	// output log
			//	char str[100];
			//	sprintf(str, "data %d: (%.2f, %.2f)", counter, packet->TankPacket.x, packet->TankPacket.y);
			//	counter++;
			//	log(str);

			//	// set other position
			//	HelloWorld::instance->_otherPlayer->setPosition(packet->TankPacket.x, packet->TankPacket.y);
			//	HelloWorld::instance->_otherPlayer->setDirection((eDirection)packet->TankPacket.direction);
			//	HelloWorld::instance->_otherPlayer->addStatus(eStatus::RUNNING);
			//}
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

void ServerConnector::sendData(const Packet & packet)
{
	// test
	if (_oldPack == packet)
		return; // ko gửi package trùng

	//_oldPack.TankPacket.direction = packet.TankPacket.direction;
	//_oldPack.TankPacket.id = packet.TankPacket.id;
	//_oldPack.TankPacket.x = packet.TankPacket.x;
	//_oldPack.TankPacket.y = packet.TankPacket.y;
	//_oldPack.TankPacket.status = packet.TankPacket.status;

	_oldPack = packet;

	WSABUF dataBuffer;
	dataBuffer.buf = (char*)&packet;
	dataBuffer.len = sizeof(Packet);
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

int ServerConnector::getServerIndex()
{
	return _serverIndex;
}

void ServerConnector::handlePacket(const Packet & packet)
{
	switch ((Packet::eType)packet.packetType)
	{
	case Packet::CREATE:
	{
		auto object = Tank::create((eObjectId)packet.CreatePacket.objectId);
		object->setPosition(packet.CreatePacket.x, packet.CreatePacket.y);
		object->setTag(packet.CreatePacket.uniqueId);

		HelloWorld::instance->addChild(object);

		break;
	}
	case Packet::OBJECT:
	{
		break;
	}
	case Packet::TANK:
	{
		auto object = (Tank*)HelloWorld::instance->getChildByTag(packet.TankPacket.id);
		if (object == nullptr)
		{
			object = Tank::create(eObjectId::YELLOW_TANK);
			object->setPosition(packet.TankPacket.x, packet.TankPacket.y);
			object->setTag(packet.TankPacket.id);
			object->setStatus((eStatus)packet.TankPacket.status);
			object->setDirection((eDirection)packet.TankPacket.direction);

			HelloWorld::instance->addChild(object);
			return;
		}

		object->setPosition(packet.TankPacket.x, packet.TankPacket.y);
		object->setStatus((eStatus)packet.TankPacket.status);
		object->setDirection((eDirection)packet.TankPacket.direction);
		
		break;
	}
	case Packet::PLAYER:
	{
		auto object = (Player*)HelloWorld::instance->getChildByName("player");
		if (object == nullptr)
			return;

		object->setTag(packet.PlayerPacket.uniqueId);
		object->setPosition(Vec2(0, 0));
		break;
	}
	default:
		break;
	}
}
