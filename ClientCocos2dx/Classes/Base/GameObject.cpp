#include "GameObject.h"
#include "SpriteManager.h"
#include "ServerConnector.h"
#include "GameObject\Tank.h"
#include "GameObject\Bullet.h"
#include "GameObject\Wall.h"

#include "..\Server\Classes\Shared\DataPacket.h"
#include "..\Server\Classes\Shared\SharedDefinitions.h"

int GameObject::INDEX_DATA_TYPE_BUFFER = 0;
int GameObject::INDEX_OBJECT_ID_BUFFER = 4;
int GameObject::INDEX_UNIQUE_ID_BUFFER = 8;
int GameObject::INDEX_STATUS_BUFFER = 12;
int GameObject::INDEX_DIRECTION_BUFFER = 16;
int GameObject::INDEX_POSITION_X_BUFFER = 17;
int GameObject::INDEX_POSITION_Y_BUFFER = 21;
int GameObject::INDEX_TIME_BUFFER = 25;

int GameObject::_nextId = 0;

GameObject * GameObject::createWithBuffer(Buffer & buffer)
{
	buffer.setBeginRead(0);

	eDataType type = (eDataType)buffer.readInt();
	if (type != eDataType::OBJECT)
		return nullptr;

	GameObject* ret = nullptr;

	eObjectId objectId = (eObjectId)buffer.readInt();
	switch (objectId)
	{
		case YELLOW_TANK:
		case GREEN_TANK:
		case WHITE_TANK:
		{
			ret = Tank::createWithBuffer(buffer);

			break;
		}
		case BULLET:
		{
			ret = Bullet::createWithBuffer(buffer);
			buffer.setBeginRead(GameObject::INDEX_POSITION_X_BUFFER);
			float x = buffer.readFloat();
			float y = buffer.readFloat();
			ret->setPosition(x, y);

			break;
		}
		case BRICK_WALL:
		{
			ret = Wall::createWithBuffer(buffer);
			break;
		}
		default:
			break;
	}

	return ret;
}

GameObject::GameObject(eObjectId id) :
	_id(id),
	_status(eStatus::NORMAL),
	_direction(eDirection::UP),
	_hasChanged(true),
	_previousBuffer(nullptr),
	_lastBuffer(nullptr)
{
	_buffer = new Buffer(BUFFER_SIZE_GAMEOBJECT);
	this->setName(SpriteManager::getInstance()->getObjectName(id));

	_lifeTime = 0.0f;
}

GameObject::GameObject(Buffer& buffer)
{
	this->initWithBuffer(buffer);
}

GameObject::~GameObject()
{
	delete _buffer;
}

void GameObject::addStatus(eStatus status)
{
	_status = (eStatus)(_status | status);
}

void GameObject::removeStatus(eStatus status)
{
	_status = (eStatus)(_status & (~status));
}

void GameObject::setStatus(eStatus status)
{
	_status = status;

	// update with status
	this->updateWithStatus(_status);
}

bool GameObject::hasStatus(eStatus status)
{
	return (_status & status) == status;
}

eStatus GameObject::getStatus()
{
	return _status;
}

void GameObject::setId(eObjectId id)
{
	_id = id;
}

eObjectId GameObject::getId()
{
	return _id;
}

void GameObject::setDirection(eDirection direction)
{
	if (_direction == direction || direction <= 0 || direction > 8)
		return;

	_direction = direction;
}

eDirection GameObject::getDirection()
{
	return _direction;
}

Buffer * GameObject::serialize()
{
	_buffer->setIndex(0);
	_buffer->setBeginRead(0);

	_buffer->writeInt(eDataType::OBJECT);
	_buffer->writeInt(this->getId());
	_buffer->writeInt(this->getUniqueId());
	_buffer->writeInt(this->getStatus());
	_buffer->writeByte(this->getDirection());
	_buffer->writeFloat(this->getPosition().x);
	_buffer->writeFloat(this->getPosition().y);
	_buffer->writeFloat(ServerConnector::getInstance()->getTime());

	return _buffer;
}

void GameObject::deserialize(Buffer & data)
{
	data.setBeginRead(0);

	eDataType type = (eDataType)data.readInt();
	if (type != eDataType::OBJECT)
		return;

	this->setType(type);
	this->setId((eObjectId)data.readInt());
	this->setTag(data.readInt());
	this->setStatus((eStatus)data.readInt());
	this->setDirection((eDirection)data.readByte());
	float x = data.readFloat();
	float y = data.readFloat();
	this->setPosition(x, y);

	auto number = data.readFloat();

	data.setBeginRead(0);
}

void GameObject::setTag(int tag)
{
	Node::setTag(tag);
	Serializable::setUniqueId(tag);
}

void GameObject::updateWithStatus(eStatus status)
{
}

void GameObject::addToPendingBuffer()
{
	auto currentBuffer = this->serialize()->clone();
	_pendingBuffer.push_back(currentBuffer);

	CCLOG("add to pending buffer: client time: %.2f", ServerConnector::getInstance()->getTime());
}

void GameObject::reconcile(Buffer &data)
{
	this->deserialize(data);
	this->updateLastBuffer(data);

	if (_previousBuffer == nullptr)
		return;

	this->interpolate();
}

void GameObject::update(float dt)
{
}

int GameObject::getNextId()
{
	_nextId += 1;
	return _nextId;
}

void GameObject::interpolate()
{
	if (_lastBuffer == nullptr || _previousBuffer == nullptr)
		return;

	_lastBuffer->setBeginRead(GameObject::INDEX_POSITION_X_BUFFER);
	float x = _lastBuffer->readFloat();
	float y = _lastBuffer->readFloat();
	Vec2 lastPos = Vec2(x, y);

	_previousBuffer->setBeginRead(GameObject::INDEX_POSITION_X_BUFFER);
	x = _previousBuffer->readFloat();
	y = _previousBuffer->readFloat();
	Vec2 prevPos = Vec2(x, y);

	_lastPosition = lastPos;
	_deltaDistance = (lastPos - prevPos);
	_nextPosition = lastPos + _deltaDistance;
}

void GameObject::updateLastBuffer(Buffer & buffer)
{
	if (_previousBuffer != nullptr)
		delete _previousBuffer;

	_previousBuffer = _lastBuffer;
	_lastBuffer = buffer.clone();
}

void GameObject::initWithBuffer(Buffer & buffer)
{
	_buffer = new Buffer(BUFFER_SIZE_GAMEOBJECT);
	this->deserialize(buffer);

	buffer.setBeginRead(buffer.getSize() - sizeof(int));

	this->setName(SpriteManager::getInstance()->getObjectName(this->getId()));

	_previousBuffer = nullptr;
	_lastBuffer = nullptr;
}
