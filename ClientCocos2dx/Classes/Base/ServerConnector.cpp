#include "ServerConnector.h"
#include "GameObject\Player.h"
#include "GameObject\Bullet.h"
#include "ClientConverterFactory.h"

//shared
#include "..\..\Server\Classes\Shared\Buffer.h"
#include "..\..\Server\Classes\Shared\DataHandler.h"
#include "..\..\Server\Classes\Shared\Serializable.h"
#include "..\..\Server\Classes\Shared\DataPacket.h"

ServerConnector* ServerConnector::_instance  = nullptr;

ServerConnector * ServerConnector::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new ServerConnector();
	}

	return _instance;
}

ServerConnector::ServerConnector()
{
}

ServerConnector::~ServerConnector()
{
	// close socket
	closesocket(_socket);
	WSACleanup();

	delete _dataHandler;
	delete _factory;
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

	_timeVal.tv_sec = 0;
	_timeVal.tv_usec = 0;

	// data handler
	_dataHandler = new DataHandler();
	_factory = new ClientConverterFactory(_dataHandler);

	_timer = 0.0f;
	_isRunning = false;

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

	CHAR buffer[DATA_BUFFER_RECIEVE_SIZE];
	WSABUF dataBuffer;
	dataBuffer.buf = buffer;
	dataBuffer.len = DATA_BUFFER_RECIEVE_SIZE;
	DWORD recvBytes;

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
		// lưu byte
		_dataHandler->recieve(dataBuffer.buf, recvBytes);
	}
}

void ServerConnector::update(float dt)
{
	if (!_connecting)
		return;

	// select
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);
	FD_SET(_socket, &_readSet);
	FD_SET(_socket, &_writeSet);

	int total = select(0, &_readSet, &_writeSet, NULL, &_timeVal);

	if (total > 0)
	{
		if (FD_ISSET(_socket, &_readSet))
		{
			this->recieveData();
		}

		if (FD_ISSET(_socket, &_writeSet))
		{
			this->sendData(_socket);
		}
	}

	_timer += dt;
}

void ServerConnector::closeConnection()
{
	closesocket(_socket);
	_connecting = false;
}

void ServerConnector::sendData(SOCKET socket)
{
	if (_dataHandler->getSendQueue(_socket) == nullptr || _dataHandler->getSendQueue(_socket)->getIndex() <= 0)
		return;

	DWORD sendBytes;
	WSABUF dataBuffer;
	dataBuffer.buf = _dataHandler->getSendQueue(_socket)->getData();
	dataBuffer.len = _dataHandler->getSendQueue(_socket)->getIndex();

	if (WSASend(_socket, &dataBuffer, 1, &sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			this->closeConnection();
		}
		return;
	}
	else
	{
		// gửi ok thì xóa
		_dataHandler->getSendQueue(_socket)->popFront(sendBytes);
	}
}

void ServerConnector::send(Serializable * object)
{
	_dataHandler->sendTo(_socket, object);
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

void ServerConnector::handleData(cocos2d::Layer* layer)
{
	auto data = _factory->convertNext();
	if (data == nullptr)
		return;

	auto type = data->getType();

	switch (type)
	{
	case eDataType::OBJECT:
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(data);
		if (gameObject)
		{
			auto object = (GameObject*)layer->getChildByTag(gameObject->getUniqueId());
			if (object == nullptr)
			{
				layer->addChild(gameObject);
				return;
			}

			//object->deserialize(*data->serialize());
			object->reconcile(*data->serialize());
		}
		break;
	}
	case eDataType::REPLY_ID: 
	{
		ReplyPacket* packet = dynamic_cast<ReplyPacket*>(data);
		if (packet)
		{
			_timer = packet->beginTime;

			auto object = (Player*)layer->getChildByName("player");
			if (object == nullptr)
				return;

			object->setTag(packet->getUniqueId());
			
			_isRunning = true;
		}

	}
	default:
		break;
	}
}

float ServerConnector::getTime()
{
	return _timer;
}

void ServerConnector::setRun(bool value)
{
	_isRunning = value;
}

bool ServerConnector::isRunning()
{
	return _isRunning;
}

ConverterFactory * ServerConnector::getFactory()
{
	return _factory;
}

void ServerConnector::setUniqueId(int id)
{
	_uniqueIdOnServer = id;
}

int ServerConnector::getUniqueId()
{
	return _uniqueIdOnServer;
}

void ServerConnector::setTime(float time)
{
	_timer = time;
}
