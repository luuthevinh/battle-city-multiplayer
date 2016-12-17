#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Vector2.h"
#include "Rect.h"
#include "..\Definitions.h"
#include "..\Shared\Serializable.h"
#include "..\GameObject\Damage.h"

class AABB;

class GameObject : public Serializable
{
public:
	static GameObject* createWithBuffer(Buffer& buffer);

	GameObject(eObjectId id);
	GameObject(Buffer& buffer);
	~GameObject();

	virtual bool init();
	virtual void update(float dt);

	virtual void setPosition(float x, float y);
	virtual void setPosition(const Vector2 &position);
	virtual const Vector2& getPosition() const;

	virtual void setDirection(eDirection direction);
	virtual eDirection getDirection();

	virtual void addStatus(eStatus status);
	virtual void removeStatus(eStatus status);
	virtual void setStatus(eStatus status);
	virtual bool hasStatus(eStatus status);
	virtual eStatus getStatus();

	virtual eObjectId getId();
	virtual void setId(eObjectId id);

	virtual void onChanged();
	virtual void setChanged(bool value);
	virtual bool hasChanged();

	// Inherited via Serializable
	virtual Buffer * serialize() override;
	virtual void deserialize(Buffer & data) override;

	virtual void handleData(Serializable* data);

	virtual Vector2 getVelocity() const;

	virtual void checkCollision(GameObject &other, float dt);

	virtual AABB* getCollisionChecker();
	
	void setCategoryBitmask(int category);
	int getCategoryBitmask();

	void setCollisionBitmask(int collsiion);
	int getCollisionBitmask();

	bool canCollisionWith(int category);

	virtual const Rect& getBoundingBox() const;
	virtual void setBoundingBox(const Rect &box);

	static int getNextUniqueId();

	virtual void gotHit(Damage* damage);

	virtual int getPacketNumber() { return _lastPacketNumber; }
	virtual void setPacketNumber(int number) { _lastPacketNumber = number; }

protected:
	Vector2 _position;
	eDirection _direction;

	eObjectId _id;
	eStatus _status;

	Rect _boudingBox;

	bool _hasChanged;

	AABB* _collisionChecker;
	int _categoryBitmask;
	int _collisionBitmask;

	static int _nextUniqueId;

	int _lastPacketNumber;
};

#endif // !__GAMEOBJECT_H__
