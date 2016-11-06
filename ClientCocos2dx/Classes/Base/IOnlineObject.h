#ifndef __IONLINE_OBJECT_H__
#define __IONLINE_OBJECT_H__

#include "Definitions.h"

class IOnlineObject
{
public:
	IOnlineObject();
	~IOnlineObject();

	// update đối tượng với packet
	virtual void updateWithPacket(const Packet &packet) = 0;

	// lấy thông tin packet
	virtual const Packet &getPacket() = 0;

protected:
	Packet _packet;
};

#endif // !__IONLINE_OBJECT_H__
