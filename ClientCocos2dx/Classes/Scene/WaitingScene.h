#ifndef __WAITING_SCENE_H__
#define __WAITING_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class WaitingScene : public Layer
{
public:
	WaitingScene();
	~WaitingScene();

	CREATE_FUNC(WaitingScene);

	static Scene* createScene();

	bool init() override;
	void update(float dt) override;

private:
	Layer* _playLayer;

	void handleData();
};

#endif // !__WAITING_SCENE_H__
