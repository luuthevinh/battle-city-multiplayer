#ifndef __SERVER_PLAY_SCENE_H__
#define __SERVER_PLAY_SCENE_H__

#include <vector>
#include "cocos2d.h"
#include "..\Server\Classes\Shared\SharedDefinitions.h"

class WorldSnapshot;
class GameObject;
class IntegerPacket;
class ClientConverterFactory;
class InfoLayer;

USING_NS_CC;

class ServerPlayScene : public Layer
{
public:
	ServerPlayScene();
	~ServerPlayScene();

	static Scene* createSceneWithLayer(Layer* layer);
	static Scene* createScene();

	CREATE_FUNC(ServerPlayScene);

	bool init() override;
	void update(float dt);

	void updateSnapshot(WorldSnapshot* snapshot);
	void handleData();
	void handleIntegerPacket(IntegerPacket* packet);

private:
	std::vector<WorldSnapshot*> _snapshots;
	std::deque<GameObject*> _tankPending;

	ClientConverterFactory* _factory;

	void initWithTMX();
	void addWall(const Vec2& position, eObjectId id);
	void addObject(GameObject* object);
	void addTank(GameObject* tank);

	GameObject* findTankPendingByUniqueId(int id);
	void removeTankPendingByUniqueId(int id);

	bool _isOver;
	void gameOver();
	void revivePlayer(int tag);

	void addChildAndAutoRelease(GameObject* object);
	InfoLayer* _infoLayer;

};

#endif // !__SERVER_PLAY_SCENE_H__
