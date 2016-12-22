#ifndef __TANK_CONVERTER_H__
#define __TANK_CONVERTER_H__

#include "SharedDefinitions.h"
#include "AStarMap.h"

NAMESPACE_TANK_BEGIN

class Converter
{
public:
	inline static eObjectId tiledIdToObjectId(int id)
	{
		switch (id)
		{
		case NONE_TILE:
			return (eObjectId)0;
		case BRICK_TILE:
			return eObjectId::BRICK_WALL;
		case GRASS_TILE:
			return eObjectId::GRASS_WALL;
		case ICE_TILE:
			return eObjectId::ICE_WALL;
		case STEEL_TILE:
			return eObjectId::STEEL_WALL;
		case WATER_TILE:
			return eObjectId::WATER_WALL;
		default:
			break;
		}
	}

	inline static const Point& positionToIndex(const Point& position)
	{
		auto x = (position.x - TILE_WIDTH) / TILE_WIDTH;
		auto y = (position.y - TILE_WIDTH) / TILE_WIDTH;

		return Point(x, y);
	}
};

NAMESPACE_TANK_END

#endif // !__TANK_CONVERTER___


