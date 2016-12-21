#ifndef __MAP_NODE_H__
#define __MAP_NODE_H__

#include "SharedDefinitions.h"

NAMESPACE_TANK_BEGIN

class Point
{
public:
	Point() {}
	Point(int xx, int yy)
	{
		x = xx;
		y = yy;
	}

	int x;
	int y;
};

class MapNode
{
public:
	MapNode(int x, int y);
	~MapNode();

	void clear();

	float getDistanceFromStart() const;
	void setDistanceFromStart(float value);
	
	float getDistanceFromTarget() const;
	void setDistanceFromTarget(float value);

	float getDistance(MapNode* node);

	float getFCost() const;

	MapNode* getParent() const;
	void setParent(MapNode* node);

	const Point& getIndex() const;
	const Point& getIndexInPosition() const;

	void setValue(int value);
	int getValue() const;

	bool operator< (const MapNode& other) 
	{ 
		return this->getFCost() < other.getFCost();
	}

	bool operator== (const MapNode& other)
	{
		return (_index.x == other.getIndex().x) && (_index.y == other.getIndex().y);
	}

private:
	Point _index;
	int _value;

	float _distanceFromStart;		// G
	float _distanceFromTarget;		// H
	float _fCost;					// F

	MapNode* _parent;
};

NAMESPACE_TANK_END

#endif // !__MAP_NODE_H__
