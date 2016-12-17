#ifndef __WORLD_SNAP_SHOT_H__
#define __WORLD_SNAP_SHOT_H__

#include <map>
#include <vector>
#include "Serializable.h"

class WorldSnapshot : public Serializable
{
public:
	static int getNextId();
	static int id;

	WorldSnapshot();
	WorldSnapshot(Buffer & data);
	~WorldSnapshot();

	// Inherited via Serializable
	virtual Buffer * serialize() override;
	virtual void deserialize(Buffer & data) override;

	std::map<int, Buffer*>& getDataObjects();
	std::vector<int> getObjectIds();
	Buffer* getObjectData(int id);

	void addObject(Serializable* object);

	void setServerTime(float time);
	float getServerTime();

	void setClientTime(float time);
	float getClientTime();

	void clearObjects();

private:


	float _serverTime;
	float _clientTime;
	int _totalSize;

	std::map<int, Buffer*> _dataObjects;

	void addBuffer(int uniqueId, Buffer* buffer);
};

#endif // !__WORLD_SNAP_SHOT__
