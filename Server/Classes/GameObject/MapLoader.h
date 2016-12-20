#ifndef __MAP_LOADER_H__
#define __MAP_LOADER_H__

#include <vector>
#include "..\Base\GameObject.h"
#include "..\tmx\tmx.h"

class MapLoader
{
public:
	MapLoader();
	~MapLoader();

	static MapLoader* createWithTMX(const char* path);

	bool initWithTMX(const char* path);

	std::vector<GameObject*> getObjectsInLayer(const char* name);

	bool hasLayer(const char* name);
	tmx_layer* getLayer(const char* name);

	int getRows();
	int getColumns();

private:
	tmx_map* _tmxMap;
};

#endif // !__MAP_LOADER_H__
