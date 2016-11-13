#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Vector2.h"
#include "Rect.h"
#include "..\Definitions.h"
#include "..\Shared\Serializable.h"


class GameObject : public Serializable
{
public:
	GameObject(eObjectId id);
	GameObject(Buffer& buffer);
	~GameObject();

	virtual bool init();
	virtual void update(float dt);

	virtual void setPosition(float x, float y);
	virtual void setPosition(const Vector2 &position);
	virtual Vector2 getPosition();

	virtual void setDirection(eDirection direction);
	virtual eDirection getDirection();

	virtual void addStatus(eStatus status);
	virtual void removeStatus(eStatus status);
	virtual void setStatus(eStatus status);
	virtual bool hasStatus(eStatus status);
	virtual eStatus getStatus();

	virtual eObjectId getId();
	virtual void setId(eObjectId id);

	virtual void setTag(int tag);
	virtual int getTag();

	virtual void onChanged();
	virtual void setChanged(bool value);
	virtual bool hasChanged();

	// Inherited via Serializable
	virtual Buffer * serialize() override;
	virtual void deserialize(Buffer & data) override;

	virtual Rect getBoundingBox() const;

	virtual Vector2 getVelocity() const;

	virtual void checkCollision(GameObject &other, float dt);

protected:
	Vector2 _position;
	eDirection _direction;

	eObjectId _id;
	eStatus _status;

	Rect _boudingBox;

	int _tag;

	bool _hasChanged;

};

#endif // !__GAMEOBJECT_H__
