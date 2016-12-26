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
	if (type != eDataType::OBJECT && type != eDataType::TANK)
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
			buffer.setBeginRead(GameObject::INDEX_POSITION_X_BUFFER);
			float x = buffer.readFloat();
			float y = buffer.readFloat();
			ret->setPosition(x, y);

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
	this->createBuffer();
	this->setName(SpriteManager::getInstance()->getObjectName(id));

	_lifeTime = 0.0f;
	_previousBuffer = new Buffer(_buffer->getSize());
	_lastBuffer = this->serialize()->clone();
	_firstUpdated = true;
}

GameObject::GameObject(Buffer& buffer)
{
	this->initWithBuffer(buffer);
}

GameObject::~GameObject()
{
	delete _buffer;
	delete _lastBuffer;
	delete _previousBuffer;
}

void GameObject::createBuffer()
{
	_buffer = new Buffer(BUFFER_SIZE_GAMEOBJECT);
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

unsigned int GameObject::getBufferSize()
{
	return BUFFER_SIZE_GAMEOBJECT;
}

void GameObject::interpolate()
{
	if (_firstUpdated)
	{
		_previousBuffer->setBeginRead(GameObject::INDEX_OBJECT_ID_BUFFER);
		int type = _previousBuffer->readInt();
		if (type == this->getId())
		{
			_firstUpdated = false;
		}

		return;
	}

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
	_previousBuffer->copy(*_lastBuffer);
	_lastBuffer->copy(buffer);
}

void GameObject::initWithBuffer(Buffer & buffer)
{
	this->createBuffer();
	this->deserialize(buffer);

	this->setName(SpriteManager::getInstance()->getObjectName(this->getId()));

	_lifeTime = 0.0f;
	_previousBuffer = new Buffer(_buffer->getSize());
	_lastBuffer = this->serialize()->clone();
	_firstUpdated = true;
}

eDirection GameObject::getIntersectSide(const Rect & other)
{
	auto myRect = this->getBoundingBox();
	auto halfWidth = myRect.size.width / 2;
	auto halfHeight = myRect.size.height / 2;
	myRect.origin.x -= halfWidth;
	myRect.origin.y -= halfHeight;

	auto otherRect = other;
	otherRect.origin.x -= other.size.width / 2;
	otherRect.origin.y -= other.size.height / 2;

	//if (!myRect.intersectsRect(otherRect))
	//	return eDirection::NONE;

	int offset = 0.0f;

	float left = otherRect.getMinX() - myRect.getMaxX();
	float top = otherRect.getMaxY() - myRect.getMinY();
	float right = otherRect.getMaxX() - myRect.getMinX();
	float bottom = otherRect.getMinY() - myRect.getMaxY();

	Vec2 deltaPosition = Vec2(otherRect.getMidX() - myRect.getMidX(), otherRect.getMidY() - myRect.getMidY());

	if (deltaPosition.x <= -halfWidth && (top > offset && bottom < -offset))
		return eDirection::LEFT;

	if (deltaPosition.x >= halfWidth && (top > offset && bottom < -offset))
		return eDirection::RIGHT;

	if (deltaPosition.y <= -halfHeight && (right > offset && left < -offset))
		return eDirection::DOWN;

	if (deltaPosition.y >= halfHeight && (right > offset && left < -offset))
		return eDirection::UP;

	return eDirection::NONE;
}
