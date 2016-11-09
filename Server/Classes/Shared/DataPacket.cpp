#include "DataPacket.h"

CommandPacket::CommandPacket()
{
	_buffer = new Buffer(13);
}

CommandPacket::CommandPacket(Buffer& data)
{
	_buffer = new Buffer(13);
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
	_buffer->writeInt(uniqueId);
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
	uniqueId = data.readInt();
	input = (eKeyInput)data.readInt();
	begin = data.readBool();

	data.setBeginRead(0);
}

ReplyPacket::ReplyPacket()
{
	_buffer = new Buffer(8);
}

ReplyPacket::ReplyPacket(Buffer &data)
{
	_buffer = new Buffer(8);
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
	_buffer->setIndex(0);

	_buffer->writeInt(eDataType::REPLY_ID);
	_buffer->writeInt(uniqueId);

	return _buffer;
}

void ReplyPacket::deserialize(Buffer & data)
{
	data.setBeginRead(0);

	eDataType type = (eDataType)data.readInt();
	if (type != eDataType::REPLY_ID)
		return;

	this->_type = type;
	this->uniqueId = data.readInt();

	data.setBeginRead(0);
}
