﻿#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <assert.h>

class Buffer
{
public:
	Buffer(unsigned int size);
	Buffer(char* data, unsigned int size);
	~Buffer();

	int getSize();

	int getIndex();
	virtual void setIndex(int index);

	virtual int getReadIndex();
	virtual void setBeginRead(int index);
	virtual bool isEndOfData();

	char* getData();

	// write next
	void writeInt(int value);
	void writeByte(char value);
	void writeBool(bool value);
	void writeFloat(float value);
	void writeData(char* data, unsigned int size);

	// read next
	int readInt();
	char readByte();
	bool readBool();
	float readFloat();
	char* readData(unsigned int size);

	Buffer* clone();
	void copy(Buffer& other);

	void setTime(float time);
	float getTime();

protected:
	char* _data;
	unsigned int _size;
	unsigned int _index;		// for write
	unsigned int _readIndex;	// for read
	float _time;
};

#endif // !__BUFFER_H__
