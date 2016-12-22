#ifndef __SERVER_PLAY_SCENE_H__
#define __SERVER_PLAY_SCENE_H__

#include <vector>
#include "cocos2d.h"
#include "..\Server\Classes\Shared\SharedDefinitions.h"

class WorldSnapshot;

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

private:
	std::vector<WorldSnapshot*> _snapshots;

	void initWithTMX();
	void addWall(const Vec2& position, eObjectId id);
};

#endif // !__SERVER_PLAY_SCENE_H__
