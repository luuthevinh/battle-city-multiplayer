#include "DataPacket.h"

CommandPacket::CommandPacket()
{
	_buffer = new Buffer(17);
	id = 0;
}

CommandPacket::CommandPacket(Buffer& data)
{
	id = 0;
	_buffer = new Buffer(17);
	this->deserialize(data);
}

CommandPacket::~CommandPacket()
{
	delete _buffer;
	_buffer = nullptr;
}

Buffer* CommandPacket::serialize()
{
	_buffer->setIndex(0);
	_buffer->setBeginRead(0);

	_buffer->writeInt(eDataType::COMMAND);
	_buffer->writeInt(id);
	_buffer->writeInt(this->getUniqueId());
	_buffer->writeInt(input);
	_buffer->writeBool(begin);

	return _buffer;
}

void CommandPacket::deserialize(Buffer &data)
{
	data.setBeginRead(0);

	eDataType type = (eDataType)data.readInt();
	if (type != eDataType::COMMAND)
		return;

	this->setType(type);

	id = data.readInt();
	this->setUniqueId(data.readInt());
	input = (eKeyInput)data.readInt();
	begin = data.readBool();

	data.setBeginRead(0);
}

ReplyPacket::ReplyPacket()
{
	_buffer = new Buffer(12);
}

ReplyPacket::ReplyPacket(Buffer &data)
{
	_buffer = new Buffer(12);
	this->deserialize(data);
}

ReplyPacket::~ReplyPacket()
{
	delete _buffer;
	_buffer = nullptr;
}

Buffer* ReplyPacket::serialize()
{
	_buffer->setIndex(0);
	_buffer->setBeginRead(0);

	_buffer->writeInt(eDataType::REPLY_ID);
	_buffer->writeInt(this->getUniqueId());
	_buffer->writeFloat(beginTime);

	return _buffer;
}

void ReplyPacket::deserialize(Buffer & data)
{
	data.setBeginRead(0);

	eDataType type = (eDataType)data.readInt();
	if (type != eDataType::REPLY_ID)
		return;

	this->_type = type;
	this->setUniqueId(data.readInt());
	this->beginTime = data.readFloat();

	data.setBeginRead(0);
}

IntegerPacket::IntegerPacket()
{
	_buffer = new Buffer(16);
}

IntegerPacket::IntegerPacket(Buffer & data)
{
	_buffer = new Buffer(16);
	this->deserialize(data);
}

Buffer * IntegerPacket::serialize()
{
	_buffer->setIndex(0);
	_buffer->setBeginRead(0);

	_buffer->writeInt(eDataType::INTEGER);
	_buffer->writeInt(integerType);
	_buffer->writeInt(this->getUniqueId());
	_buffer->writeInt(value);

	return _buffer;
}

void IntegerPacket::deserialize(Buffer & data)
{
	data.setBeginRead(0);

	auto type = (eDataType)data.readInt();
	if (type != eDataType::INTEGER)
		return;

	_type = type;
	this->integerType = (IntegerPacket::Type)data.readInt();
	this->setUniqueId(data.readInt());
	this->value = data.readInt();

	data.setBeginRead(0);
}

RoomInfo::RoomInfo()
{
}

RoomInfo::RoomInfo(Buffer & data)
{
	this->deserialize(data);
}

Buffer * RoomInfo::serialize()
{
	if (_buffer != nullptr)
		delete _buffer;

	_totalSize = playerCounters.size() * sizeof(int) * 2;
	_buffer = new Buffer(_totalSize + 8);

	_buffer->setIndex(0);
	_buffer->setBeginRead(0);

	_buffer->writeInt(eDataType::ROOM_INFO);
	_buffer->writeInt(this->getUniqueId());

	for (auto it = playerCounters.begin(); it != playerCounters.end(); it++)
	{
		_buffer->writeInt(it->first);
		_buffer->writeInt(it->second);
	}

	return _buffer;
}

void RoomInfo::deserialize(Buffer & data)
{
	data.setBeginRead(0);

	auto type = (eDataType)data.readInt();
	if (type != eDataType::ROOM_INFO)
		return;

	_type = type;
	this->setUniqueId(data.readInt());
	
	while (!data.isEndOfData())
	{
		auto id = (eObjectId)data.readInt();
		auto count = data.readInt();

		playerCounters[id] = count;
	}

	data.setBeginRead(0);
}
