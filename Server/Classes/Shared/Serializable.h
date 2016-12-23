#ifndef __ISERIALIZABLE_H__
#define __ISERIALIZABLE_H__

#include "..\Definitions.h"
#include "Buffer.h"

class Serializable
{
public:
	Serializable() { _buffer = nullptr; }
	~Serializable() { }

	virtual Buffer* serialize() = 0;

	virtual void deserialize(Buffer& data) = 0;

	virtual inline eDataType getType() { return _type; }
	virtual inline void setType(eDataType type) { _type = type; }

	virtual inline void setUniqueId(int id) { _uniqueId = id; }
	virtual inline int getUniqueId() const { return _uniqueId; }

protected:
	eDataType _type;
	Buffer* _buffer;
	int _uniqueId;

};


#endif // !__ISERIALIZABLE_H__
