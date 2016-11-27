#include "GameObject.h"
#include "SpriteManager.h"
#include "ServerConnector.h"

GameObject::GameObject(eObjectId id) : 
	_id(id),
	_status(eStatus::NORMAL),
	_direction(eDirection::UP)
{
	_buffer = new Buffer(29);
	this->setName(SpriteManager::getInstance()->getObjectName(id));

	_lifeTime = 0.0f;
}

GameObject::GameObject(Buffer& buffer)
{
	_buffer = new Buffer(29);
	this->deserialize(buffer);

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
	_buffer->writeInt(this->getTag());
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

	_lifeTime = data.readFloat();

	data.setBeginRead(0);
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

	data.setBeginRead(data.getSize() - 4);
	float time = data.readFloat();

	int index = _pendingBuffer.size() - 1;
	for (index; index >= 0; index--)
	{
		_pendingBuffer[index]->setBeginRead(_pendingBuffer[index]->getSize() - 4);
		auto t = _pendingBuffer[index]->readFloat();

		// thời gian nhận được sau pending thì cập nhật lại từ đây
		if (time >= t)
		{
			this->deserialize(data);
			break;
		}
	}

	// xóa tất cả thằng trc đó đi
	for (auto i = 0; i <= index; i++)
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
	//if (_currentPendingBufferIndex < _pendingBuffer.size() - 1)
	//{
	//	this->reconcilePendingBuffer();
	//}
	//else
	//{
	//	this->predict(dt);

	//	if (ServerConnector::instance->isRunning())
	//	{
	//		this->addToPendingBuffer();
	//	}
	//}

	// this->predict(dt);
}