﻿#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Vector2.h"
#include "../Definitions.h"

class GameObject
{
public:
	GameObject(eObjectId id);
	~GameObject();

	virtual bool init();
	virtual void update(float dt);

	virtual void setPosition(float x, float y);
	virtual void setPosition(const Vector2 &position);
	virtual Vector2 getPosition();

	virtual void addStatus(eStatus status);
	virtual void setStatus(eStatus status);
	virtual bool hasStatus(eStatus status);
	virtual eStatus getStatus();

	virtual eObjectId getId();
	virtual void setTag(int tag);
	virtual int getTag();

	virtual void onChanged();
	virtual void setChanged(bool value);
	virtual bool hasChanged();

	Packet getPacket();
protected:
	Vector2 _position;

	eObjectId _id;
	eStatus _status;

	int _tag;

	bool _hasChanged;

	// giữ packet gửi đi trong đây luôn
	Packet _packet;
};

#endif // !__GAMEOBJECT_H__