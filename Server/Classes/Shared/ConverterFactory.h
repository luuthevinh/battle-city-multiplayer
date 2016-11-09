#ifndef __CONVERTER_FACTORY_H__
#define __CONVERTER_FACTORY_H__

#include "DataHandler.h"

class ConverterFactory
{
public:
	ConverterFactory(DataHandler* handler);
	~ConverterFactory();

	virtual Serializable* convertNext() = 0;

protected:
	DataHandler* _handlerRef;
};

#endif // !__CONVERTER_FACTORY_H__
