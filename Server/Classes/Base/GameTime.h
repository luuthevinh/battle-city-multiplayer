#ifndef __GAMETIME_H__
#define __GAMETIME_H__

#include <ctime>

class GameTime
{
public:
	GameTime();
	~GameTime();

	void init();
	void update();

	// Tổng thời gian trôi qua kể từ lần cuối gọi update
	float getElapsedTime();	

	// Tổng thời gian trôi qua kể từ lúc gọi Init
	float getTotalTime();

	clock_t getTicks();

private:
	float _elapsedTime;
	float _totalTime;

	clock_t _totalTicks;
};

#endif // !__TIMER_H__
