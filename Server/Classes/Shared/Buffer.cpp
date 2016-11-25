#include "Buffer.h"
#include <cstring>

Buffer::Buffer(unsigned int size)
{
	_size = size;
	_data = new char[size];
	_index = 0;
	_readIndex = 0;
}

Buffer::Buffer(char* data, unsigned int size)
{
	_size = size;
	_data = data;
	_index = 0;
	_readIndex = 0;
}

Buffer::~Buffer() 
{ 
	delete[] _data; 
}

int Buffer::getSize() 
{ 
	return _size; 
}

int Buffer::getIndex() 
{ 
	return _index; 
}

void Buffer::setIndex(int index) 
{ 
	_index = index; 
}

void Buffer::setBeginRead(int index)
{
	_readIndex = index;
}

char* Buffer::getData() 
{ 
	return _data; 
}

void Buffer::writeInt(int value)
{
	assert(this->getIndex() + sizeof(int) <= this->getSize());

	// little endian
	*((int*)(this->getData() + this->getIndex())) = value;

	this->setIndex(this->getIndex() + sizeof(int));
}

void Buffer::writeByte(char value)
{
	assert(this->getIndex() + sizeof(char) <= this->getSize());

	// little endian
	*((char*)(this->getData() + this->getIndex())) = value;

	this->setIndex(this->getIndex() + sizeof(char));
}

void Buffer::writeBool(bool value)
{
	assert(this->getIndex() + sizeof(bool) <= this->getSize());

	// little endian
	*((bool*)(this->getData() + this->getIndex())) = value;

	this->setIndex(this->getIndex() + sizeof(bool));
}

void Buffer::writeFloat(float value)
{
	assert(this->getIndex() + sizeof(float) <= this->getSize());

	// little endian
	*((float*)(this->getData() + this->getIndex())) = value;

	this->setIndex(this->getIndex() + sizeof(float));
}

int Buffer::readInt()
{
	assert(_readIndex + sizeof(int) <= this->getSize());

	int ret = 0;

	ret = *(int*)(this->getData() + _readIndex);

	_readIndex += sizeof(int);

	return ret;
}

char Buffer::readByte()
{
	assert(_readIndex + sizeof(char) <= this->getSize());

	char ret;

	ret = *(char*)(this->getData() + _readIndex);

	_readIndex += sizeof(char);

	return ret;
}

bool Buffer::readBool()
{
	assert(_readIndex + sizeof(bool) <= this->getSize());

	bool ret;

	ret = *(bool*)(this->getData() + _readIndex);

	_readIndex += sizeof(bool);

	return ret;
}

float Buffer::readFloat()
{
	assert(_readIndex + sizeof(float) <= this->getSize());

	float ret;

	ret = *(float*)(this->getData() + _readIndex);

	_readIndex += sizeof(float);

	return ret;
}

Buffer * Buffer::clone()
{
	Buffer* ret = new Buffer(this->getSize());
	// copy byte array
	std::memcpy(ret->getData(), this->getData(), this->getSize());

	return ret;
}
