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

	inline static const char* directionToString(const eDirection& direction)
	{
		switch (direction)
		{
		case eDirection::LEFT:
			return "left";
		case eDirection::RIGHT:
			return "right";
		case eDirection::UP:
			return "up";
		case eDirection::DOWN:
			return "down";
		default:
			return "none";
		}
	}

	inline static const char* tankLevelToString(const eTankLevel& level)
	{
		switch (level)
		{
		case DEFAULT_TANK:
			return "_default_";
		case SECOND_TANK:
			return "_second_";
		case THIRD_TANK:
			return "_third_";
		case FOURTH_TANK:
			return "_fourth_";
		case BASIC_TANK:
			return "_basic_";
		case FAST_TANK:
			return "_fast_";
		case POWER_TANK:
			return "_power_";
		case ARMOR_TANK:
			return "_armor_";
		default:
			return "_default_";
		}
	}
};

NAMESPACE_TANK_END

#endif // !__TANK_CONVERTER___


