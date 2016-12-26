#include "ClientConverterFactory.h"
#include "GameObject\Tank.h"
#include "GameObject\Bullet.h"
#include "GameObject\Wall.h"

#include "..\Server\Classes\Shared\DataPacket.h"
#include "..\Server\Classes\Shared\WorldSnapshot.h"

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

	Serializable * ret = nullptr;
	auto readQueue = _handlerRef->getReadQueue();

	// thằng đầu ko phải kích thước là int (4 bytes) thì lỗi cnmr
	ASSERT_MSG(readQueue->getIndex() >= 4, "read data should begin with size");

	int size = *(int*)readQueue->popFront(4);
	char* data = readQueue->readFront(size);

	Buffer* buffer = new Buffer(data, size);
	eDataType type = (eDataType)buffer->readInt();

	switch (type)
	{
	case OBJECT:
	{
		ret = GameObject::createWithBuffer(*buffer);
		break;
	}
	case TANK:
	{
		ret = Tank::createWithBuffer(*buffer);
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
	readQueue->popFront(size);

	return ret;
}
