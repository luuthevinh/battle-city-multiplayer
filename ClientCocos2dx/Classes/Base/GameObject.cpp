#include "GameObject.h"
#include "SpriteManager.h"
#include "ServerConnector.h"
#include "GameObject\Tank.h"
#include "GameObject\Bullet.h"
#include "GameObject\Wall.h"

#include "..\Server\Classes\Shared\DataPacket.h"

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
	_packetNumber(0)
{
	_buffer = new Buffer(29);
	this->setName(SpriteManager::getInstance()->getObjectName(id));

	_lifeTime = 0.0f;
}

GameObject::GameObject(Buffer& buffer)
{
	_buffer = new Buffer(29);
	this->deserialize(buffer);

	buffer.setBeginRead(buffer.getSize() - sizeof(int));
	_packetNumber = buffer.readInt();

	this->setName(SpriteManager::getInstance()->getObjectName(this->getId()));
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
	_buffer->writeInt(_packetNumber);

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

	auto number = data.readInt();

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
	_currentPendingBufferIndex = _pendingBuffer.size() - 1;
}

void GameObject::reconcile(Buffer &data)
{
	if (_pendingBuffer.size() <= 0)
	{
		this->deserialize(data);
		return;
	}

	this->deserialize(data);

	data.setBeginRead(data.getSize() - 4);
	auto time = data.readInt();

	int index = 0;
	for (index; index < _pendingBuffer.size(); index++)
	{
		_pendingBuffer[index]->setBeginRead(_pendingBuffer[index]->getSize() - 4);
		auto t = _pendingBuffer[index]->readInt();

		// thời gian nhận được sau pending thì cập nhật lại từ đây
		if (time <= t)
		{
			this->deserialize(*_pendingBuffer[index]);
		}
	}

	while (!_pendingBuffer.empty())
	{
		delete _pendingBuffer.front();
		_pendingBuffer.pop_front();
	}

	_currentPendingBufferIndex = 0;
}

void GameObject::reconcilePendingBuffer()
{
	if (_pendingBuffer.size() <= 0)
		return;

	this->deserialize(*_pendingBuffer.front());

	delete _pendingBuffer.front();
	_pendingBuffer.pop_front();
}

void GameObject::update(float dt)
{
	this->predict(dt);

	if (ServerConnector::getInstance()->isRunning() && this->hasChanged())
	{
		_packetNumber++;
		this->addToPendingBuffer();
		this->onChanged(false);
	}
}