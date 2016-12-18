#include "Utils.h"

float tank::lerp(float goal, float current, float delta)
{
	float distance = goal - current;

	if (distance > delta)
		return current + delta;

	if (distance < -delta)
		return current - delta;

	return goal;
}
