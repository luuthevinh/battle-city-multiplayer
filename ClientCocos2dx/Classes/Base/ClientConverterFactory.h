#ifndef __CLIENT_CONVERTER_FACTORY_H__
#define __CLIENT_CONVERTER_FACTORY_H__

#include "..\Server\Classes\Shared\ConverterFactory.h"

class Serializable;

class ClientConverterFactory : public ConverterFactory
{
public:
	ClientConverterFactory(DataHandler* handler);
	~ClientConverterFactory();

	// Inherited via ConverterFactory
	virtual Serializable * convertNext() override;
private:



};

#endif // !__CLIENT_CONVERTER_FACTORY_H__
