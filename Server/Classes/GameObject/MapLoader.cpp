#include "MapLoader.h"
#include "Wall.h"

MapLoader::MapLoader()
{
}

MapLoader::~MapLoader()
{
	tmx_map_free(_tmxMap);
}

MapLoader* MapLoader::createWithTMX(const char * path)
{
	auto loader = new(std::nothrow) MapLoader();
	if (loader && loader->initWithTMX(path))
	{
		return loader;
	}

	delete loader;
	return nullptr;
}

bool MapLoader::initWithTMX(const char * path)
{
	_tmxMap = tmx_load(path);

	if (_tmxMap == nullptr)
	{
		tmx_perror("tmx_load");
		return false;
	}

}

std::vector<GameObject*> MapLoader::getObjectsInLayer(const char * name)
{
	std::vector<GameObject*> objects;
	
	auto layer = this->getLayer(name);
	if (layer == nullptr)
		return objects;

	float offset = _tmxMap->tile_width / 2;

	for (size_t i = 0; i < _tmxMap->width * _tmxMap->height; i++)
	{
		int row = i / _tmxMap->width;
		int col = i % _tmxMap->width;

		auto position = Vector2(_tmxMap->tile_width * col + offset, WINDOW_HEIGHT - (_tmxMap->tile_height * row + offset));
		GameObject* object = nullptr;

		auto id = layer->content.gids[i];

		switch (id)
		{
		case 1:
		{
			object = Wall::createWithPosition(eObjectId::BRICK_WALL, position);
			break;
		}
		case 2:
		{
			object = Wall::createWithPosition(eObjectId::STEEL_WALL, position);
			break;
		}
		default:
			break;
		}

		if (object != nullptr)
		{
			objects.push_back(object);
		}
	}

	return objects;
}

bool MapLoader::hasLayer(const char * name)
{
	if (_tmxMap == nullptr)
	{
		return false;
	}

	auto layer = _tmxMap->ly_head;
	while (layer)
	{
		if (std::strcmp(layer->name, name) == 0)
		{
			return true;
		}

		layer = layer->next;
	}

	return false;
}

tmx_layer * MapLoader::getLayer(const char * name)
{
	if (_tmxMap == nullptr)
	{
		return nullptr;
	}

	auto layer = _tmxMap->ly_head;
	while (layer)
	{
		if (std::strcmp(layer->name, name) == 0)
		{
			return layer;
		}

		layer = layer->next;
	}

	return nullptr;
}

int MapLoader::getRows()
{
	return _tmxMap->height;
}

int MapLoader::getColumns()
{
	return _tmxMap->width;
}
