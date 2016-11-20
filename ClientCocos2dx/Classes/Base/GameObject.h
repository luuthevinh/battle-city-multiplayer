#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "cocos2d.h"
#include "Definitions.h"

USING_NS_CC;

// shared
#include "..\Server\Classes\Shared\Serializable.h"

class GameObject : public Node, public Serializable
{
public:
	GameObject(eObjectId id);
	GameObject(Buffer& buffer);
	~GameObject();

	virtual void addStatus(eStatus status);
	virtual void removeStatus(eStatus status);
	virtual void setStatus(eStatus status);
	virtual eStatus getStatus();

	virtual void setId(eObjectId id);
	virtual eObjectId getId();

	virtual void setDirection(eDirection direction);
	virtual eDirection getDirection();

	// Inherited via Serializable
	virtual Buffer * serialize() override;
	virtual void deserialize(Buffer & data) override;

	virtual void updateWithStatus(eStatus status);
	
	virtual void reconcile(Buffer &data);

	virtual void update(float dt) override;
	virtual void predict(float dt) {};
protected:
	Sprite* _sprite;

	eObjectId _id;
	eStatus _status;

	eDirection _direction;

	float _lifeTime;
	std::deque<Buffer*> _pendingBuffer;		// trạng thái dự đoán chưa được server xác nhận
	int _currentPendingBufferIndex;
	
	void addToPendingBuffer();
	void reconcilePendingBuffer();
};


#endif // !__GAMEOBJECT_H__
