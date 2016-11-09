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

enum eDataType {
	OBJECT = 1,
	PACKET = 2,
	REPLY_ID = 3,
	COMMAND = 4,
};

#define TANK_NORMAL_VELOCITY 100

#endif // !__SHARED_DEFINITIONS_H__
