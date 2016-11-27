#include "Damage.h"

Damage::Damage() : 
	_value(1),
	_fromObject(eObjectId::YELLOW_TANK)
{
}

Damage::~Damage()
{
}

Damage * Damage::create(eObjectId object, int value, eDirection direction)
{
	auto ret = new Damage();
	ret->setValue(value);
	ret->setFromObject(object);
	ret->setDirection(direction);

	return ret;
}

int Damage::getValue()
{
	return _value;
}

void Damage::setValue(int value)
{
	_value = value;
}

void Damage::setFromObject(eObjectId id)
{
	_fromObject = id;
}

eObjectId Damage::getObjectId()
{
	return _fromObject;
}

void Damage::setDirection(eDirection direction)
{
	_direction = direction;
}

eDirection Damage::getDirection()
{
	return _direction;
}
