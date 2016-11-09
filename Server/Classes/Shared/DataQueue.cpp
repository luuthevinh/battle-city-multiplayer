#include "DataQueue.h"

using namespace std;

DataQueue::DataQueue(unsigned int size) : Buffer(size)
{
}

DataQueue::~DataQueue()
{
}

void DataQueue::pushBack(char * data, unsigned int size)
{
	assert(_index + size < _size);

	memcpy(_data + _index, data, size);

	_index += size;
}

char* DataQueue::popFront(int size)
{
	assert(_index >= size);

	// get data
	char* data = new char[size];
	memcpy(data, _data, size);

	// pop
	memcpy(_data, _data + size, _size);
	_index -= size;

	return data;
}

char * DataQueue::readFront(int size)
{
	assert(_index >= size);

	// get data
	char* data = new char[size];
	memcpy(data, _data, size);

	return data;
}

void DataQueue::setIndex(int index)
{
	_index = index;
}

