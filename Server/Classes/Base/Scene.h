#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include "GameObject.h"
#include "..\GameObject\Player.h"

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
	virtual void removeObject(int tag);

	virtual int addPlayer(int socketIndex);
	virtual void removePlayer(int socketIndex);
	virtual Player* getPlayer(int tag);

protected:
	std::vector<GameObject*> _gameObjects;
	std::vector<Player*> _players;

	int _uniqueIdCounter;
};

#endif // !__SCENE_H__
