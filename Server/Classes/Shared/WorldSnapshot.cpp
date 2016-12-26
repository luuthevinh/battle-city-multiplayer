#include "WorldSnapshot.h"
#include "..\Server.h"

int WorldSnapshot::id = 0;

int WorldSnapshot::getNextId()
{
	id++;
	return id;
}

WorldSnapshot::WorldSnapshot() :
	_serverTime(0),
	_clientTime(0),
	_totalSize(0)
{

}

WorldSnapshot::WorldSnapshot(Buffer & data) : 
	_serverTime(0),
	_clientTime(0),
	_totalSize(0)
{
	this->deserialize(data);
}

WorldSnapshot::~WorldSnapshot()
{
	this->clearObjects();
}

Buffer * WorldSnapshot::serialize()
{
	if (_buffer != nullptr)
		delete _buffer;

	int newSize = _totalSize + 16;
	_buffer = new Buffer(newSize);

	_buffer->writeInt(eDataType::SNAPSHOT);
	_buffer->writeInt(this->getUniqueId());
	_buffer->writeFloat(_serverTime);
	_buffer->writeFloat(_clientTime);

	// object data
	for (auto it = _dataObjects.begin(); it != _dataObjects.end(); it++)
	{
		_buffer->writeData(it->second->getData(), it->second->getSize());
	}

	return _buffer;
}

void WorldSnapshot::deserialize(Buffer & data)
{
	data.setBeginRead(0);

	auto type = (eDataType)data.readInt();
	if (type != eDataType::SNAPSHOT)
		return;

	this->setType(type);
	this->setUniqueId(data.readInt());
	_serverTime = data.readFloat();
	_clientTime = data.readFloat();

	while (!data.isEndOfData())
	{
		auto objectType = data.readInt();
		if (objectType == eDataType::OBJECT)
		{
			data.setBeginRead(data.getReadIndex() + sizeof(int)); // int kế tiếp là object id, tạm bỏ qua
			auto uniqueId = data.readInt();
			data.setBeginRead(data.getReadIndex() - sizeof(int) * 3);

			auto buffer = new Buffer(data.readData(BUFFER_SIZE_GAMEOBJECT), BUFFER_SIZE_GAMEOBJECT);
			this->addBuffer(uniqueId, buffer);
		}
		else if (objectType == eDataType::TANK)
		{
			data.setBeginRead(data.getReadIndex() + sizeof(int)); // int kế tiếp là object id, tạm bỏ qua
			auto uniqueId = data.readInt();
			data.setBeginRead(data.getReadIndex() - sizeof(int) * 3);

			auto buffer = new Buffer(data.readData(BUFFER_SIZE_TANK), BUFFER_SIZE_TANK);
			this->addBuffer(uniqueId, buffer);
		}
	}
}

std::map<int, Buffer*>& WorldSnapshot::getDataObjects()
{
	return _dataObjects;
}

std::vector<int> WorldSnapshot::getObjectIds()
{
	std::vector<int> ret;
	for (auto i = _dataObjects.begin(); i != _dataObjects.end(); i++)
	{
		ret.push_back(i->first);
	}

	return ret;
}

Buffer * WorldSnapshot::getObjectData(int id)
{
	if(_dataObjects.find(id) == _dataObjects.end())
		return nullptr;

	return _dataObjects.at(id);
}

void WorldSnapshot::addObject(Serializable* object)
{
	this->addBuffer(object->getUniqueId(), object->serialize()->clone());
}

void WorldSnapshot::setServerTime(float time)
{
	_serverTime = time;
}

float WorldSnapshot::getServerTime()
{
	return _serverTime;
}

void WorldSnapshot::setClientTime(float time)
{
	_clientTime = time;
}

float WorldSnapshot::getClientTime()
{
	return _clientTime;
}

void WorldSnapshot::clearObjects()
{
	for (auto it = _dataObjects.begin(); it != _dataObjects.end(); it++)
	{
		delete it->second;
	}
	_dataObjects.clear();
	_totalSize = 0;
}

void WorldSnapshot::addBuffer(int uniqueId, Buffer* buffer)
{
	if(_dataObjects.find(uniqueId) != _dataObjects.end())
	{
		_totalSize -= _dataObjects[uniqueId]->getSize();
		delete _dataObjects[uniqueId];
	}

	_totalSize += buffer->getSize();
	_dataObjects[uniqueId] = buffer;
}
