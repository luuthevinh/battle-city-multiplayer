#ifndef __SERVER_PLAY_SCENE_H__
#define __SERVER_PLAY_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class ServerPlayScene : public Layer
{
public:
	ServerPlayScene();
	~ServerPlayScene();

	static Scene* createSceneWithLayer(Layer* layer);

	CREATE_FUNC(ServerPlayScene);

	bool init() override;
	void update(float dt);

private:

};

#endif // !__SERVER_PLAY_SCENE_H__
