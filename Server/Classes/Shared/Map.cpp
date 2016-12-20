#include "Map.h"
#include <algorithm>

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

std::vector<MapNode*> AStarMap::findPath(MapNode* start, MapNode* target)
{
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
	auto bx = start.x / 16;
	auto by = (WINDOW_WIDTH - start.y) / 16 - 1;
	auto beginNode = this->getNode(bx, by);

	auto ex = end.x / 16;
	auto ey = (WINDOW_WIDTH - end.y) / 16 - 1;
	auto endNode = this->getNode(ex, ey);

	return this->findPath(beginNode, endNode);
}

MapNode * AStarMap::getNode(int x, int y)
{
	if(x * y >= _nodes.size() || x * y < 0)
		return nullptr;

	return _nodes[y * _columns + x];
}

std::vector<MapNode*> AStarMap::getNeigbour(MapNode * node)
{
	auto result = std::vector<MapNode*>();

	auto currentIndex = node->getIndex();

	if (currentIndex.y > 1)
	{
		int index = (currentIndex.y - 1) * _columns + currentIndex.x;
		auto n = _nodes.at(index);

		//auto top = Point(currentIndex.x, currentIndex.y - 2);
		//auto topNode = this->getNode(top.x, top.y);

		if (n && this->canMove(node) /*&& this->canMove(topNode)*/)
		{
			result.push_back(n);
		}
	}

	if (currentIndex.y < _columns - 2)
	{
		int index = (currentIndex.y + 1) * _columns + currentIndex.x;
		auto n = _nodes.at(index);

		//auto bot = Point(currentIndex.x, currentIndex.y + 2);
		//auto botNode = this->getNode(bot.x, bot.y);

		if (n && this->canMove(node) /*&& this->canMove(botNode)*/)
		{
			result.push_back(n);
		}
	}

	if (currentIndex.x > 1)
	{
		int index = currentIndex.y * _columns + (currentIndex.x - 1);
		auto n = _nodes.at(index);

		//auto left = Point(currentIndex.x - 2, currentIndex.y);
		//auto leftNode = this->getNode(left.x, left.y);

		if (n && this->canMove(node) /*&& this->canMove(leftNode)*/)
		{
			result.push_back(n);
		}
	}

	if (currentIndex.x < _rows - 2)
	{
		int index = currentIndex.y * _columns + (currentIndex.x + 1);
		auto n = _nodes.at(index);

		//auto right = Point(currentIndex.x + 2, currentIndex.y);
		//auto rightNode = this->getNode(right.x, right.y);

		if (n && this->canMove(node)/* && this->canMove(rightNode)*/)
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

NAMESPACE_TANK_END