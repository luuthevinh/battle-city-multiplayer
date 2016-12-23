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
	static int INDEX_DATA_TYPE_BUFFER;
	static int INDEX_OBJECT_ID_BUFFER;
	static int INDEX_UNIQUE_ID_BUFFER;
	static int INDEX_STATUS_BUFFER;
	static int INDEX_DIRECTION_BUFFER;
	static int INDEX_POSITION_X_BUFFER;
	static int INDEX_POSITION_Y_BUFFER;
	static int INDEX_TIME_BUFFER;

	static GameObject* createWithBuffer(Buffer& buffer);

	GameObject(eObjectId id);
	GameObject(Buffer& buffer);
	~GameObject();

	virtual void addStatus(eStatus status);
	virtual void removeStatus(eStatus status);
	virtual void setStatus(eStatus status);
	virtual bool hasStatus(eStatus status);
	virtual eStatus getStatus();

	virtual void setId(eObjectId id);
	virtual eObjectId getId();

	virtual void setDirection(eDirection direction);
	virtual eDirection getDirection();

	// Inherited via Serializable
	virtual Buffer * serialize() override;
	virtual void deserialize(Buffer & data) override;

	virtual void setTag(int tag) override;

	virtual void updateWithStatus(eStatus status);
	
	virtual void reconcile(Buffer &data);

	virtual void update(float dt) override;
	virtual void predict(float dt) {};

	virtual bool hasChanged() { return _hasChanged; }
	virtual void onChanged(bool value = true) { _hasChanged = value; }

	static int getNextId();
protected:
	static int _nextId;

	Sprite* _sprite;

	eObjectId _id;
	eStatus _status;

	eDirection _direction;

	float _lifeTime;
	std::deque<Buffer*> _pendingBuffer;		// trạng thái dự đoán chưa được server xác nhận
	
	void addToPendingBuffer();

	bool _hasChanged;
	bool _firstUpdated;

	Buffer* _lastBuffer;
	Buffer* _previousBuffer;

	Vec2 _lastPosition;
	Vec2 _nextPosition;
	Vec2 _deltaDistance;

	void interpolate();
	void updateLastBuffer(Buffer& buffer);

	void initWithBuffer(Buffer& buffer);
};


#endif // !__GAMEOBJECT_H__
