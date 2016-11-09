#include "DataHandler.h"
#include "..\GameObject\Tank.h"

DataHandler::DataHandler()
{
	_readQueue = new DataQueue(DATA_BUFFER_SIZE);
}

DataHandler::~DataHandler()
{
	delete _readQueue;

	for (auto data : _sendDataQueues)
	{
		delete data.second;
	}

	_sendDataQueues.clear();
}

void DataHandler::sendTo(SOCKET socket, char * data, unsigned int size)
{
	if (_sendDataQueues.find(socket) == _sendDataQueues.end())
	{
		// chưa có thì tạo mới
		_sendDataQueues[socket] = new DataQueue(DATA_BUFFER_SIZE);
	}

	// kích thước
	_sendDataQueues[socket]->pushBack((char*)&size, sizeof(int));
	// xong tới dữ liệu
	_sendDataQueues[socket]->pushBack(data, size);
}

void DataHandler::sendTo(SOCKET socket, Buffer & buffer)
{
	this->sendTo(socket, buffer.getData(), buffer.getIndex());
}

void DataHandler::sendTo(SOCKET socket, Serializable* object)
{
	auto buffer = object->serialize();
	this->sendTo(socket, *buffer);
}

void DataHandler::recieve(char * data, unsigned int size)
{
	_readQueue->pushBack(data, size);
}

DataQueue * DataHandler::getReadQueue()
{
	return _readQueue;
}

DataQueue * DataHandler::getSendQueue(SOCKET socket)
{
	if (_sendDataQueues.find(socket) != _sendDataQueues.end())
		return _sendDataQueues.at(socket);

	return nullptr;
}
