#include "MapNode.h"
#include <math.h>
#include <random>

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

void MapNode::clear()
{
	_distanceFromStart = 0;
	_distanceFromTarget = 0;
	_fCost = 0;
	_parent = nullptr;
}

float MapNode::getDistanceFromStart() const
{
	return _distanceFromStart;
}

void MapNode::setDistanceFromStart(float value)
{
	_distanceFromStart = value;
	_fCost = _distanceFromStart + _distanceFromTarget + rand() % 3;
}

float MapNode::getDistanceFromTarget() const
{
	return _distanceFromTarget;
}

void MapNode::setDistanceFromTarget(float value)
{
	_distanceFromTarget = value;
	_fCost = _distanceFromStart + _distanceFromTarget + rand() % 3;
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

const Point & MapNode::getIndex() const
{
	return _index;
}

const Point & MapNode::getIndexInPosition() const
{
	Point position;

	position.x = (_index.x + 1) * TILE_WIDTH;
	position.y = WINDOW_HEIGHT - (_index.y + 1) * TILE_WIDTH;

	return position;
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