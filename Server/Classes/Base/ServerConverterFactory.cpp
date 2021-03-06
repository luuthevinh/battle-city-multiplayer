﻿#include "ServerConverterFactory.h"
#include "..\Base\GameObject.h"
#include "..\Shared\DataPacket.h"

ServerConverterFactory::ServerConverterFactory(DataHandler* handler) : ConverterFactory(handler)
{
}

ServerConverterFactory::~ServerConverterFactory()
{
}

Serializable * ServerConverterFactory::convertNext()
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
		case PACKET:
			break;
		case REPLY_ID:
			break;
		case COMMAND:
		{
			ret = new CommandPacket(*buffer);
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
