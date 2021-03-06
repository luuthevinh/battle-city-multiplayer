#ifndef __SHARED_DEFINITIONS_H__
#define __SHARED_DEFINITIONS_H__

#include <stdint.h>

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define TILE_WIDTH 16

#define TANK_NORMAL_VELOCITY 80
#define TANK_FAST_VELOCITY 112
#define TANK_SLOW_VELOCITY 64

#define BULLET_SPEED_01 200
#define BULLET_SPEED_02 250
#define BULLET_SPEED_03 300

#define BUFFER_SIZE_GAMEOBJECT 29
#define BUFFER_SIZE_TANK 33

#define SEND_NUMBER_PER_SEC 10

#define BULLET_SIZE_WIDTH 6
#define TANK_SIZE_WIDTH 32

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
	STEEL_WALL = 1 << 6,
	GRASS_WALL = 1 << 7,
	ICE_WALL = 1 << 8,
	WATER_WALL = 1 << 9,
	EAGLE = 1 << 10,
	ALL = 0xFFFFFFF
};

enum eTankLevel
{
	DEFAULT_TANK = 1,
	SECOND_TANK = 2,
	THIRD_TANK = 3,
	FOURTH_TANK = 4,
	BASIC_TANK = 5,
	FAST_TANK = 6,
	POWER_TANK = 7,
	ARMOR_TANK = 8
};

enum eBulletLevel
{
	SLOW_BULLET = 1,
	NORMAL_BULLET = 2,
	FAST_BULLET = 3
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
	SNAPSHOT = 6,
	ROOM_INFO = 7,
	TANK = 8
};

enum eTiledObjectId 
{
	NONE_TILE = 0,
	BRICK_TILE = 1,
	GRASS_TILE = 2,
	ICE_TILE = 3,
	STEEL_TILE = 4,
	WATER_TILE = 5,
};

#define NAMESPACE_TANK_BEGIN namespace tank {
#define NAMESPACE_TANK_END }

#define USING_NS_TANK using namespace tank;

#endif // !__SHARED_DEFINITIONS_H__
