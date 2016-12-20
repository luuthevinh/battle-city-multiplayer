#ifndef __ASTAR_MAP_H__
#define __ASTAR_MAP_H__
#include <vector>
#include "SharedDefinitions.h"
#include "MapNode.h"

NAMESPACE_TANK_BEGIN

class AStarMap
{
public:
	AStarMap(int row, int col);
	~AStarMap();

	void setValue(const Point& index, int value);

	std::vector<MapNode*> findPath(MapNode* start, MapNode* target);
	std::vector<MapNode*> findPath(const Point& start, const Point& end);
	
	MapNode* getNode(int x, int y);

private:
	std::vector<MapNode*> _nodes;

	int _rows;
	int _columns;

	std::vector<MapNode*> getNeigbour(MapNode* node);
	MapNode* minCostNode(std::vector<MapNode*>& nodes);
	
	bool canMove(MapNode* node);
};

NAMESPACE_TANK_END

#endif // !__ASTAR_MAP_H__
