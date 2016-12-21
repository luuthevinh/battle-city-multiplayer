#ifndef __SHARED_DEFINITIONS_H__
#define __SHARED_DEFINITIONS_H__

#include <stdint.h>

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define TILE_WIDTH 16

#define TANK_NORMAL_VELOCITY 96
#define BULLET_SPEED_01 200
#define BUFFER_SIZE_GAMEOBJECT 29
#define SEND_NUMBER_PER_SEC 10

enum eDirection
{
	NONE = 0,
	LEFT = 1 << 0,
	UP = 1 << 1,
	RIGHT = 1 << 2,
	DOWN = 1 << 3
};

enum eObjectId
{
	YELLOW_TANK = 1 << 0,
	GREEN_TANK = 1 << 1,
	WHITE_TANK = 1 << 2,
	BULLET = 1 << 3,
	EXPLOSION = 1 << 4,
	BRICK_WALL = 1 << 5,
	STEEL_WALL = 1 << 6
};

enum eTankLevel
{
	BASIC_TANK = 1,
	FAST_TANK = 2,
	POWER_TANK = 3,
	ARMOR_TANK = 4
};

enum eStatus
{
	DIE = 0,
	NORMAL = 1 << 0,
	PROTECTED = 1 << 1,
	RUNNING = 1 << 2,
	STAND = 1 << 3,
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
	INTEGER = 5,
	SNAPSHOT = 6
};

enum eTiledObjectId 
{
	NONE_TILE = 0,
	BRICK_TILE = 1,
	STELL_TILE = 4
};

#define NAMESPACE_TANK_BEGIN namespace tank {
#define NAMESPACE_TANK_END }

#define USING_NS_TANK using namespace tank;

#endif // !__SHARED_DEFINITIONS_H__
