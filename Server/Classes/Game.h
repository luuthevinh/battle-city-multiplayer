#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include "Base\GameObject.h"
#include "GameObject\Player.h"
#include "Shared\ConverterFactory.h"

class Game
{
public:
	static Game* instance;

	Game();
	~Game();

	bool init();
	void update(float dt);
	void destroy();

	float getFrameRate();

	void handleData(ConverterFactory* factory);

private:
	float _frameRate;
};

#endif // !__GAME_H__
