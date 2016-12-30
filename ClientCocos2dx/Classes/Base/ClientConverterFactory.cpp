#include "ClientConverterFactory.h"
#include "GameObject\Tank.h"
#include "GameObject\Bullet.h"
#include "GameObject\Wall.h"

#include "..\Server\Classes\Shared\DataPacket.h"
#include "..\Server\Classes\Shared\WorldSnapshot.h"
#include "ServerConnector.h"

#include <thread>

ClientConverterFactory::ClientConverterFactory(DataHandler * handler) : ConverterFactory(handler)
{
}

ClientConverterFactory::~ClientConverterFactory()
{
}

Serializable * ClientConverterFactory::convertNext()
{
	if (_handlerRef == nullptr)
		return nullptr;

	if (_handlerRef->getReadQueue() == nullptr || _handlerRef->getReadQueue()->getIndex() <= 0)
		return nullptr;

	CCLOG("read size: %d", _handlerRef->getReadQueue()->getIndex());

	Serializable * ret = nullptr;
	auto readQueue = _handlerRef->getReadQueue();

	// thằng đầu ko phải kích thước là int (4 bytes) thì lỗi cnmr
	ASSERT_MSG(readQueue->getIndex() >= 4, "read data should begin with size");

	int size = *(int*)this->popDataRead(4);;
	char* data = readQueue->readFront(size);

	Buffer* buffer = new Buffer(data, size);
	eDataType type = (eDataType)buffer->readInt();

	switch (type)
	{
	case OBJECT:
	{
		ret = GameObject::createInfo(*buffer);
		break;
	}
	case TANK:
	{
		ret = Tank::createInfo(*buffer);
		break;
	}
	case REPLY_ID:
	{
		ret = new ReplyPacket(*buffer);
		break;
	}
	case COMMAND:
	{
		ret = new CommandPacket(*buffer);
		break;
	}
	case SNAPSHOT: 
	{
		ret = new WorldSnapshot(*buffer);
		break;
	}
	case ROOM_INFO:
	{
		ret = new RoomInfo(*buffer);
		break;
	}
	case INTEGER: 
	{
		ret = new IntegerPacket(*buffer);
		break;
	}
	default:
		break;
	}

	// ko dùng buffer này nữa
	delete buffer;
	this->popDataRead(size);

	return ret;
}

Serializable * ClientConverterFactory::getNext()
{
	if (!_convertedObjects.empty())
	{
		//std::lock_guard<std::mutex> guard(_mutex);
		auto ret = _convertedObjects.front();
		_convertedObjects.pop_front();
		return ret;
	}
		
	return nullptr;
}

void ClientConverterFactory::startConvertMultithread()
{
	std::thread thread1(&ClientConverterFactory::convertNextDataInQueue, this);
	thread1.detach();
}

void ClientConverterFactory::convertNextDataInQueue()
{
	std::lock_guard<std::mutex> guard(_mutex);
	auto object = this->convertNext();

	if (object != nullptr)
	{
		CCLOG("push converted object from other thread.");
		_convertedObjects.push_back(object);
	}
}

void ClientConverterFactory::pushDataRead(char * data, int size)
{
	//std::lock_guard<std::mutex> guard(_mutexReadQueue);
	_handlerRef->recieve(data, size);
}

char * ClientConverterFactory::popDataRead(int size)
{
	//std::lock_guard<std::mutex> guard(_mutexReadQueue);
	auto data = _handlerRef->getReadQueue()->popFront(size);
	return data;
}
