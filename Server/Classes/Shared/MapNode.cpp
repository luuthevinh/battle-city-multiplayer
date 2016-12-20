#include "MapNode.h"
#include <math.h>

NAMESPACE_TANK_BEGIN

MapNode::MapNode(int x, int y) :
	_parent(nullptr),
	_value(0)
{
	_index.x = x;
	_index.y = y;
}

MapNode::~MapNode()
{
}

float MapNode::getDistanceFromStart() const
{
	return _distanceFromStart;
}

void MapNode::setDistanceFromStart(float value)
{
	_distanceFromStart = value;
	_fCost = _distanceFromStart + _distanceFromTarget;
}

float MapNode::getDistanceFromTarget() const
{
	return _distanceFromTarget;
}

void MapNode::setDistanceFromTarget(float value)
{
	_distanceFromTarget = value;
	_fCost = _distanceFromStart + _distanceFromTarget;
}

float MapNode::getDistance(MapNode * node)
{
	return abs(_index.x - node->getIndex().x) + abs(_index.y - node->getIndex().y);
}

float MapNode::getFCost() const
{
	return _fCost;
}

MapNode * MapNode::getParent() const
{
	return _parent;
}

void MapNode::setParent(MapNode * node)
{
	_parent = node;
}

Point MapNode::getIndex() const
{
	return _index;
}

void MapNode::setValue(int value)
{
	_value = value;
}

int MapNode::getValue() const
{
	return _value;
}


NAMESPACE_TANK_END