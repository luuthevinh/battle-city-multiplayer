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
	virtual void setStatus(eStatus status);
	virtual eStatus getStatus();

	virtual void setId(eObjectId id);
	virtual eObjectId getId();

	virtual void setDirection(eDirection direction);
	virtual eDirection getDirection();

	// Inherited via Serializable
	virtual Buffer * serialize() override;
	virtual void deserialize(Buffer & data) override;

protected:
	Sprite* _sprite;

	eObjectId _id;
	eStatus _status;

	eDirection _direction;
};


#endif // !__GAMEOBJECT_H__
