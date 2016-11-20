#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include "Base\GameObject.h"
#include "GameObject\Player.h"
#include "Shared\ConverterFactory.h"
#include "Base\GameTime.h"

class Game
{
public:
	static Game* instance;
	static Game* createWithConverter(ConverterFactory* factory);

	Game(ConverterFactory* factory);
	~Game();

	bool init();
	void run();
	void update(float dt);
	void destroy();

	float getFrameRate();

	void handleData();
	
	void setConverterFactory(ConverterFactory* factory);
	
	GameTime* getGameTime();

private:
	float _frameRate;

	GameTime* _gameTime;
	float _detalTime;
	float _lastTime;

	ConverterFactory* _factory;
};

#endif // !__GAME_H__
