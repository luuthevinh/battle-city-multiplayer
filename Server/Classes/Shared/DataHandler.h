#ifndef __DATA_HANDLER_H__
#define __DATA_HANDLER_H__

#include <map>
#include "..\Shared\DataQueue.h"
#include "..\Definitions.h"
#include "..\Shared\Serializable.h"

class DataHandler
{
public:
	DataHandler();
	~DataHandler();

	void sendTo(SOCKET socket, char* data, unsigned int size);
	void sendTo(SOCKET socket, Buffer& buffer);
	void sendTo(SOCKET socket, Serializable* object);

	void recieve(char* data, unsigned int size);

	DataQueue* getReadQueue();
	DataQueue* getSendQueue(SOCKET socket);

private:
	DataQueue* _readQueue;
	std::map<SOCKET, DataQueue*> _sendDataQueues;
};


#endif // !__DATA_HANDLER_H__
