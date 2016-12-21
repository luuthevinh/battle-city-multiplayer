#include "Utils.h"

NAMESPACE_TANK_BEGIN

float lerp(float goal, float current, float delta)
{
	float distance = goal - current;

	if (distance > delta)
		return current + delta;

	if (distance < -delta)
		return current - delta;

	return goal;
}

NAMESPACE_TANK_END
