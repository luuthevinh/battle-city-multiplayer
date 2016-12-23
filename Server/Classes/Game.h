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
	void handleDataFromWaitingRoom(Serializable* data);
	void handleIntegerPacket(IntegerPacket* integer);
	void setConverterFactory(ConverterFactory* factory);
	
	GameTime* getGameTime();
	int getNumberOfBots();

private:
	float _frameRate;

	GameTime* _gameTime;
	float _detalTime;
	float _lastTime;

	int _totalBots;
	int _maxBots;

	ConverterFactory* _factory;
	void updateBots(int value);

	void createPlayer(const Player& info);
};

#endif // !__GAME_H__
