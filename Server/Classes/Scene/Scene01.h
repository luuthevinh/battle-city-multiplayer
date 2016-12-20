#ifndef __SCENE_01_H__
#define __SCENE_01_H__

#include <vector>
#include "..\Base\GameObject.h"
#include "..\Base\Scene.h"

#include "..\Shared\Map.h"

class Scene01 : public Scene
{
public:
	Scene01();
	~Scene01();

	// Inherited via Scene
	virtual bool init() override;

	virtual void update(float dt) override;

	virtual void destroy() override;

	void handleData(Serializable * object) override;

	virtual void sendInitDataTo(SOCKET socket) override;

	virtual int addPlayer(int socketIndex) override;

private:
	void checkCollisionObjects(float dt);
	void checkStatusObjects();
	void sendChangedObjects();
	void updateSnapshot(Serializable * object);

	tank::AStarMap* _aStarMap;
};


#endif // !__SCENE_01_H__
