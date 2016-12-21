#include "AStarMap.h"
#include <algorithm>
#include <time.h>

NAMESPACE_TANK_BEGIN

AStarMap::AStarMap(int row, int col)
{
	_rows = row;
	_columns = col;

	for (size_t i = 0; i < row * col; i++)
	{
		int x = i % col;
		int y = i / col;

		_nodes.push_back(new MapNode(x, y));
	}
}

AStarMap::~AStarMap()
{
	while (!_nodes.empty())
	{
		delete _nodes.back();
		_nodes.pop_back();
	}
}

void AStarMap::setValue(const Point & index, int value)
{
	int i = index.y * _columns + index.x;
	if (i > _nodes.size())
		return;

	_nodes[i]->setValue(value);
}

void AStarMap::refresh()
{
	for (auto node : _nodes)
	{
		node->clear();
	}
}

std::vector<MapNode*> AStarMap::findPath(MapNode* start, MapNode* target)
{
	this->refresh();

	auto result = std::vector<MapNode*>();
	auto openNodes = std::vector<MapNode*>();
	auto closedNodes = std::vector<MapNode*>();
	
	start->setParent(nullptr);
	start->setDistanceFromStart(0);
	start->setDistanceFromTarget(start->getDistance(target));
	
	openNodes.push_back(start);

	while (!openNodes.empty())
	{
		auto current = this->minCostNode(openNodes);

		openNodes.erase(std::find(openNodes.begin(), openNodes.end(), current));
		closedNodes.push_back(current);

		if (current == target)
		{
			break;
		}

		auto neighbours = this->getNeigbour(current);

		for (auto neighbour : neighbours)
		{
			auto isInClosed = std::find(closedNodes.begin(), closedNodes.end(), neighbour);
			if (isInClosed != closedNodes.end())
			{
				continue;
			}

			auto isInOpen = std::find(openNodes.begin(), openNodes.end(), neighbour);
			
			auto newCost = current->getDistanceFromStart() + current->getDistance(neighbour);

			if (isInOpen == openNodes.end() || neighbour->getDistanceFromStart() > newCost)
			{
				neighbour->setParent(current);
				neighbour->setDistanceFromStart(neighbour->getDistance(start));
				neighbour->setDistanceFromTarget(neighbour->getDistance(target));

				if (isInOpen == openNodes.end())
				{
					openNodes.push_back(neighbour);
				}
			}
		}

	}

	result.push_back(target);
	auto parent = target->getParent();
	while (parent != nullptr)
	{
		result.push_back(parent);
		parent = parent->getParent();
	}

	return result;
}

std::vector<MapNode*> AStarMap::findPath(const Point & start, const Point & end)
{
	auto startIndex = this->positionToIndex(start);
	auto beginNode = this->getNode(startIndex.x, startIndex.y);

	auto endIndex = this->positionToIndex(end);
	auto endNode = this->getNode(endIndex.x, endIndex.y);

	return this->findPath(beginNode, endNode);
}

MapNode * AStarMap::getNode(int x, int y)
{
	if(x * y >= _nodes.size() || x * y < 0)
		return nullptr;

	return _nodes[y * _columns + x];
}

MapNode * AStarMap::getRandomNodeHasValue(int value)
{
	srand(time(NULL));
	bool error = false;

	MapNode * ret = nullptr;
	do
	{
		int row = rand() % _rows;
		int col = rand() % _columns;
		ret =  this->getNode(row, col);

		if (ret == nullptr || ret->getValue() != value)
		{
			error = true;
		}

	} while (error);

	return ret;
}

const Point & AStarMap::positionToIndex(const Point & position)
{
	auto x = (position.x - TILE_WIDTH) / TILE_WIDTH;
	auto y = (WINDOW_HEIGHT - position.y - TILE_WIDTH) / TILE_WIDTH;

	if ((position.x - TILE_WIDTH) % TILE_WIDTH != 0)
		x++;
	
	if ((WINDOW_HEIGHT - position.y - TILE_WIDTH) % TILE_WIDTH != 0)
		y++;

	if (x < 0) x = 0;
	if (x > _columns - 1) x = _columns - 1;
	if (y < 0) y = 0;
	if (y > _rows - 1) y = _rows - 1;
	
	return Point(x, y);
}

std::vector<MapNode*> AStarMap::getNeigbour(MapNode * node)
{
	auto result = std::vector<MapNode*>();
	auto currentIndex = node->getIndex();

	if (currentIndex.y > 0)
	{
		// top
		auto n = this->getNode(currentIndex.x, currentIndex.y - 1);

		if (n && this->canMove(n) && this->canMoveRight(n))
		{
			result.push_back(n);
		}
	}

	if (currentIndex.y < _rows - 2)
	{
		// bot
		auto n = this->getNode(currentIndex.x, currentIndex.y + 1);

		if (n && this->canMove(n) && this->canMoveBottom(n) && this->canMoveRightBottom(n))
		{
			result.push_back(n);
		}
	}

	if (currentIndex.x > 0)
	{
		// left
		auto n = this->getNode(currentIndex.x - 1, currentIndex.y);

		if (n && this->canMove(n) && this->canMoveRightBottom(n))
		{
			result.push_back(n);
		}
	}

	if (currentIndex.x < _columns - 2)
	{
		// right
		auto n = this->getNode(currentIndex.x + 1, currentIndex.y);

		if (n && this->canMove(n) && this->canMoveRight(n) && this->canMoveRightBottom(n))
		{
			result.push_back(n);
		}
	}

	return result;
}

MapNode * AStarMap::minCostNode(std::vector<MapNode*>& nodes)
{
	MapNode* min = nodes[0];

	for (auto node : nodes)
	{
		if (node->getFCost() > node->getFCost())
		{
			min = node;
		}
	}

	return min;
}

bool AStarMap::canMove(MapNode * node)
{
	auto cur = node->getIndex();
	if(node->getValue() != 0)
		return false;

	return true;
}

bool AStarMap::canMoveRight(MapNode * node)
{
	auto index = node->getIndex();
	auto right = Point(index.x + 1, index.y);
	if (right.x > _columns - 1)
		return false;

	auto rightNode = this->getNode(right.x, right.y);

	return this->canMove(rightNode);
}

bool AStarMap::canMoveBottom(MapNode * node)
{
	auto index = node->getIndex();
	auto bot = Point(index.x, index.y + 1);
	if (bot.y > _rows - 1)
		return false;

	auto botNode = this->getNode(bot.x, bot.y);

	return this->canMove(botNode);
}

bool AStarMap::canMoveRightBottom(MapNode * node)
{
	auto index = node->getIndex();
	auto rightBot = Point(index.x + 1, index.y + 1);
	if (rightBot.y > _rows - 1 || rightBot.x > _columns - 1)
		return false;

	auto rightBotNode = this->getNode(rightBot.x, rightBot.y);

	return this->canMove(rightBotNode);
}

NAMESPACE_TANK_END