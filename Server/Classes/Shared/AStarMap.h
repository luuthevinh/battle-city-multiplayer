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
	void refresh();

	std::vector<MapNode*> findPath(MapNode* start, MapNode* target);
	std::vector<MapNode*> findPath(const Point& start, const Point& end);
	
	MapNode* getNode(int x, int y);
	MapNode* getRandomNodeHasValue(int value);

	const Point& positionToIndex(const Point& position);

	void addTempObstacle(const Point& index);
	void clearTempObstacle();
	bool isInTempObstacle(const Point& index);

private:
	std::vector<MapNode*> _nodes;
	std::vector<Point> _tempObstales;

	int _rows;
	int _columns;

	std::vector<MapNode*> getNeigbour(MapNode* node);
	MapNode* minCostNode(std::vector<MapNode*>& nodes);
	
	bool canMove(MapNode* node);
	bool canMoveRight(MapNode* node);
	bool canMoveBottom(MapNode* node);
	bool canMoveRightBottom(MapNode* node);
};

NAMESPACE_TANK_END

#endif // !__ASTAR_MAP_H__
