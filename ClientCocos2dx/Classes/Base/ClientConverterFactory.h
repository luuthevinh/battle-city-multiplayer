#ifndef __CLIENT_CONVERTER_FACTORY_H__
#define __CLIENT_CONVERTER_FACTORY_H__

#include "..\Server\Classes\Shared\ConverterFactory.h"

#include <deque>
#include <mutex>

class Serializable;

class ClientConverterFactory : public ConverterFactory
{
public:
	ClientConverterFactory(DataHandler* handler);
	~ClientConverterFactory();

	// Inherited via ConverterFactory
	virtual Serializable * convertNext() override;

	Serializable* getNext();

	void startConvertMultithread();
	void convertNextDataInQueue();

	void pushDataRead(char* data, int size);
	char* popDataRead(int size);

private:
	std::deque<Serializable*> _convertedObjects;

	std::mutex _mutex;
	std::mutex _mutexReadQueue;

};

#endif // !__CLIENT_CONVERTER_FACTORY_H__
