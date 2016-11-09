#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include "Base\GameObject.h"
#include "GameObject\Player.h"

class Game
{
public:
	static Game* instance;

	Game();
	~Game();

	bool init();

	void update(float dt);

	void update(char* data);

	float getFrameRate();

	const std::vector<GameObject*>& getAllGameObjects() const;
	const std::vector<Player*>& getAllPlayers() const;

	int addObject(GameObject* object);
	GameObject* getGameObject(eObjectId id);
	void removeObject(int tag);

	void addPlayer(int index);
	void removePlayer(int index);
	Player* getPlayer(int index);

	void handlePlayerInput(int playerTag, eKeyInput input, bool start = true);
	void handleData(Serializable* object);

private:
	std::vector<GameObject*> _gameObjects;
	std::vector<Player*> _players;

	float _frameRate;
	int _uniqueIdCounter;
};

#endif // !__GAME_H__
