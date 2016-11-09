#ifndef __ISERIALIZABLE_H__
#define __ISERIALIZABLE_H__

#include "..\Definitions.h"
#include "Buffer.h"

class Serializable
{
public:
	Serializable() { }
	~Serializable() { }

	virtual Buffer* serialize() = 0;

	virtual void deserialize(Buffer& data) = 0;

	virtual eDataType getType() { return _type; };
	virtual void setType(eDataType type) { _type = type; };

protected:
	eDataType _type;
	Buffer* _buffer;
};


#endif // !__ISERIALIZABLE_H__
