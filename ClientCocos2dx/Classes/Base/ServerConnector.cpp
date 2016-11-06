#include "ServerConnector.h"
#include "Scene\HelloWorldScene.h"
#include "GameObject\Player.h"
#include "GameObject\Bullet.h"

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
				this->closeConnection();
			}
			return;
		}
		else
		{
			// read data buffer with recvBytes
			if (recvBytes < dataBuffer.len)
				return;

			// lấy packet xử lý
			Packet* packet = (Packet*)dataBuffer.buf;
			this->handlePacket(*packet);
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
	if (_oldPack == packet)
		return; // ko gửi package trùng

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
	//case Packet::CREATE:
	//{
	//	auto object = Tank::create((eObjectId)packet.CreatePacket.objectId);
	//	object->setPosition(packet.CreatePacket.x, packet.CreatePacket.y);
	//	object->setTag(packet.CreatePacket.uniqueId);

	//	HelloWorld::instance->addChild(object);

	//	break;
	//}
	case Packet::OBJECT:
	{
		auto object = (GameObject*)HelloWorld::instance->getChildByTag(packet.ObjectPacket.uniqueId);
		if (object == nullptr)
		{
			switch ((eObjectId)packet.ObjectPacket.objectType)
			{
			case eObjectId::BULLET:
			{
				auto pos = Vec2(packet.ObjectPacket.x, packet.ObjectPacket.y);

				auto bullet = Bullet::create(pos, (eDirection)packet.ObjectPacket.direction);
				bullet->setTag(packet.ObjectPacket.uniqueId);
				bullet->setStatus((eStatus)packet.ObjectPacket.status);
				
				HelloWorld::instance->addChild(bullet);

				return;
			}
			default:
				break;
			}
		}

		object->setStatus((eStatus)packet.ObjectPacket.status);

		auto pos = Vec2(packet.ObjectPacket.x, packet.ObjectPacket.y);
		object->setPosition(pos);

		break;
	}
	case Packet::TANK:
	{
		auto object = (Tank*)HelloWorld::instance->getChildByTag(packet.TankPacket.id);
		if (object == nullptr)
		{
			auto tank = Tank::create(eObjectId::YELLOW_TANK);
			tank->setPosition(packet.TankPacket.x, packet.TankPacket.y);
			tank->setTag(packet.TankPacket.id);
			tank->setStatus((eStatus)packet.TankPacket.status);
			tank->setDirection((eDirection)packet.TankPacket.direction);

			HelloWorld::instance->addChild(tank);
			return;
		}

		object->updateWithPacket(packet);

		break;
	}
	case Packet::PLAYER:
	{
		auto object = (Player*)HelloWorld::instance->getChildByName("player");
		if (object == nullptr)
			return;

		// lấy tag làm unique id cho object
		object->setTag(packet.PlayerPacket.uniqueId);
		object->setPosition(Vec2(0, 0));
		break;
	}
	default:
		break;
	}
}
