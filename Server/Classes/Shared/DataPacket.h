#ifndef __DATA_PACKET_H__
#define __DATA_PACKET_H__

#include "Serializable.h"
#include <map>

class CommandPacket : public Serializable
{
public:
	CommandPacket();
	CommandPacket(Buffer &data);
	~CommandPacket();

	// Inherited via Serializable
	virtual Buffer* serialize() override;

	virtual void deserialize(Buffer &data) override;

	int id;
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
};

class RoomInfo : public Serializable

{
public:
	RoomInfo();
	RoomInfo(Buffer &data);

	// Inherited via Serializable
	virtual Buffer * serialize() override;
	virtual void deserialize(Buffer & data) override;

	std::map<eObjectId, int> playerCounters;

private:
	int _totalSize;
};

#endif // !__DATA_PACKET_H__
