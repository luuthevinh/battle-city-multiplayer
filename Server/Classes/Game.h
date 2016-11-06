#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include "Base\GameObject.h"
#include "GameObject\Player.h"

class Game
{
public:
	Game();
	~Game();

	bool init();

	void update(float dt);

	void update(char* data);

	float getFrameRate();

	const std::vector<GameObject*>& getAllGameObjects() const;
	const std::vector<Player*>& getAllPlayers() const;

	GameObject* getGameObject(eObjectId id);

	void addPlayer(int index);
	void removePlayer(int index);
	Player* getPlayer(int index);

private:
	std::vector<GameObject*> _gameObjects;
	std::vector<Player*> _players;

	float _frameRate;
};

#endif // !__GAME_H__
