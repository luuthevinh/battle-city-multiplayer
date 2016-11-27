#ifndef __DAMAGE_H__
#define __DAMAGE_H__

#include "..\Definitions.h"

class Damage
{
public:
	Damage();
	~Damage();

	static Damage* create(eObjectId object, int value, eDirection direction);

	int getValue();
	void setValue(int value);

	void setFromObject(eObjectId id);
	eObjectId getObjectId();

	void setDirection(eDirection direction);
	eDirection getDirection();

private:
	int _value;
	eDirection _direction;
	eObjectId _fromObject;
};

#endif // !__DAMAGE_H__
