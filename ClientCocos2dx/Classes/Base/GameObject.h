#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "cocos2d.h"
#include "Definitions.h"

USING_NS_CC;

class GameObject : public Node
{
public:
	GameObject(eObjectId id);
	~GameObject();

	virtual void addStatus(eStatus status);
	virtual void setStatus(eStatus status);
	virtual eStatus getStatus();

protected:
	Sprite* _sprite;

	eObjectId _id;
	eStatus _status;

};


#endif // !__GAMEOBJECT_H__
