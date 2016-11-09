#ifndef __SERVER_CONVERTER_FACTORY_H__
#define __SERVER_CONVERTER_FACTORY_H__

#include "..\Shared\ConverterFactory.h"

class ServerConverterFactory : public ConverterFactory
{
public:
	ServerConverterFactory(DataHandler* handler);
	~ServerConverterFactory();

	// Inherited via ConverterFactory
	virtual Serializable * convertNext() override;

private:

};

#endif // !__SERVER_CONVERTER_FACTORY_H__
