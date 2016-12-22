#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include "../Server/Classes/Shared/SharedDefinitions.h"

#define PORT 1495
#define SERVER_ADD "127.0.0.1"

enum eAnimateId
{
	YELLOW_TANK_LEFT,
	YELLOW_TANK_UP,
	YELLOW_TANK_RIGHT,
	YELLOW_TANK_BOTTOM
};

enum eSpriteId 
{
	SPRITE_BULLET
};

#define DEFAULT_ANIMATION_TIME 0.16

#endif // !__DEFINITIONS_H__
