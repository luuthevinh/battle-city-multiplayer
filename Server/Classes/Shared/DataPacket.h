#ifndef __DATA_PACKET_H__
#define __DATA_PACKET_H__

#include "Serializable.h"

class CommandPacket : public Serializable
{
public:
	CommandPacket();
	CommandPacket(Buffer &data);
	~CommandPacket();

	// Inherited via Serializable
	virtual Buffer* serialize() override;

	virtual void deserialize(Buffer &data) override;

	int uniqueId;
	eKeyInput input;
	bool begin;

};

class ReplyPacket : public Serializable
{
public:
	ReplyPacket();
	ReplyPacket(Buffer &data);
	~ReplyPacket();

	// Inherited via Serializable
	virtual Buffer* serialize() override;

	virtual void deserialize(Buffer &data) override;

	int uniqueId;

};

#endif // !__DATA_PACKET_H__
