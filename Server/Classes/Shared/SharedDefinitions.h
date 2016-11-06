#ifndef __SHARED_DEFINITIONS_H__
#define __SHARED_DEFINITIONS_H__

#include <stdint.h>

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

enum eDirection
{
	LEFT = 1,
	UP = 2,
	RIGHT = 3,
	DOWN = 4
};

enum eObjectId
{
	YELLOW_TANK = 1 << 0,
	GREEN_TANK = 1 << 1,
	WHITE_TANK = 1 << 2,
	BULLET = 1 << 3,
	EXPLOSION = 1 << 4
};

enum eStatus
{
	DIE = 0,
	NORMAL = 1 << 0,
	PROTECTED = 1 << 1,
	RUNNING = 1 << 2,
	STAND = 1 << 3,
};

enum eLevel
{
	LEVEL01,
	LEVEL02,
};

enum eKeyInput
{
	KEY_NONE,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
	KEY_SHOOT
};

#define TANK_NORMAL_VELOCITY 100

struct Packet
{
	enum eType { TANK, OBJECT, COMMAND, PLAYER, EMPTY };	// 4 bytes

	uint8_t packetType;				// 1

	uint32_t fromSocket;			// 1

	union
	{
		struct _ObjectPacket
		{
			uint8_t uniqueId;				// 1 tag
			uint8_t direction;				// 1
			uint8_t status;					// 1
			uint32_t objectType;			// 4
			float x;						// 4
			float y;						// 4
		} ObjectPacket;

		struct _TankPacket
		{
			uint8_t id;				// 1
			uint8_t direction;		// 1
			uint8_t status;			// 1
			float x;				// 4
			float y;				// 4
		} TankPacket;

		struct _CommandPacket
		{
			uint8_t fromObject;	// uniqued id của object
			uint8_t objectId;
			float x;
			float y;
		} CommandPacket;

		struct _Player
		{
			uint8_t uniqueId;
			uint8_t playerInput;
			bool start;
		} PlayerPacket;
	};

	bool operator==(Packet pack) const
	{
		if (pack.packetType != packetType)
			return false;

		switch (pack.packetType)
		{
		case Packet::TANK:
		{
			if (pack.TankPacket.x != TankPacket.x || 
				pack.TankPacket.y != TankPacket.y || 
				pack.TankPacket.id != TankPacket.id ||
				pack.TankPacket.status != TankPacket.status || 
				pack.TankPacket.direction != TankPacket.direction)
				return false;

			return true;
		}
		case Packet::OBJECT:
		{
			if (pack.ObjectPacket.x != ObjectPacket.x ||
				pack.ObjectPacket.y != ObjectPacket.y ||
				pack.ObjectPacket.uniqueId != ObjectPacket.uniqueId || 
				pack.ObjectPacket.status != ObjectPacket.status ||
				pack.ObjectPacket.direction != ObjectPacket.direction)
				return false;

			return true;
		}
		default:
			break;
		}
		return false;
	}
};

#endif // !__SHARED_DEFINITIONS_H__
