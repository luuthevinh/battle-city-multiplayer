#ifndef __BUFFER_H__
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
	virtual void setBeginRead(int index);

	char* getData();

	// write next
	void writeInt(int value);
	void writeByte(char value);
	void writeBool(bool value);
	void writeFloat(float value);

	// read next
	int readInt();
	char readByte();
	bool readBool();
	float readFloat();

	Buffer* clone();

protected:
	char* _data;
	unsigned int _size;
	unsigned int _index;		// for write
	unsigned int _readIndex;	// for read
};

#endif // !__BUFFER_H__
