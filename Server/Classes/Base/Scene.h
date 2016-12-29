#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include "GameObject.h"
#include "..\GameObject\Player.h"

#include "..\Shared\WorldSnapshot.h"

class Scene
{
public:
	Scene();
	~Scene();

	virtual bool init() = 0;
	virtual void update(float dt) = 0;
	virtual void destroy() = 0;

	virtual void handleData(Serializable* object);

	virtual int addObject(GameObject* object);
	virtual int addStaticObject(GameObject* object);

	virtual void removeObject(int tag);

	virtual int addPlayer(int socketIndex);
	virtual void addPlayer(Player* player);

	virtual void removePlayer(int socketIndex);
	virtual Player* getPlayer(int tag);

	virtual void sendInitDataTo(SOCKET socket);

	virtual WorldSnapshot* getSnapshot();

	virtual const Vector2& getPlayerStartPosition();

	virtual void beginGame() { }

	virtual int getNumberOfBots() { return 0; };
	virtual void setOver(bool over) { _isOver = over; };

protected:
	std::vector<GameObject*> _gameObjects;
	std::vector<GameObject*> _staticObjects;

	std::vector<Player*> _players;

	WorldSnapshot* _snapshot;
	bool _isOver;
};

#endif // !__SCENE_H__
