#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

enum eObjectId
{
	YELLOW_TANK = 1 << 0,
	GREEN_TANK = 1 << 1,
	WHITE_TANK = 1 << 2,
};

enum eDirection
{
	LEFT,
	TOP,
	RIGHT,
	BOTTOM
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

enum eAnimateId
{
	YELLOW_TANK_LEFT,
	YELLOW_TANK_UP,
	YELLOW_TANK_RIGHT,
	YELLOW_TANK_BOTTOM
};

enum eSpriteId 
{
	
};

#define DEFAULT_ANIMATION_TIME 0.1
#define TANK_NORMAL_VELOCITY 100

#endif // !__DEFINITIONS_H__