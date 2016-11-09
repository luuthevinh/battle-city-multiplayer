#ifndef __DATA_QUEUE_H___
#define __DATA_QUEUE_H___

#include <assert.h>
#include <cstring>

#include "Buffer.h"

class DataQueue : public Buffer
{
public:
	DataQueue(unsigned int size);
	~DataQueue();

	void pushBack(char* data, unsigned int size);
	char* popFront(int size);

	char* readFront(int size);

private:
	virtual void setIndex(int index) override;
};

#endif // !__DATA_QUEUE_H___
