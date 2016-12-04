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
	float beginTime;

};


class IntegerPacket : public Serializable
{
public:
	enum Type
	{
		PLAYER_CHARACTER_SELECTION = 1,
		READY
	};

	IntegerPacket();
	IntegerPacket(Buffer &data);

	// Inherited via Serializable
	virtual Buffer* serialize() override;
	virtual void deserialize(Buffer &data) override;

	Type integerType;
	int value;
	int uniqueId;
};

#endif // !__DATA_PACKET_H__
