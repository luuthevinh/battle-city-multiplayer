#ifndef __WAITING_SCENE_H__
#define __WAITING_SCENE_H__

#include "cocos2d.h"
#include "GameObject\TankCursor.h"

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

	void onKeyPressed(EventKeyboard::KeyCode keycode, Event* e);

private:
	Layer* _playLayer;
	TankCursor* _cursor;

	bool _isReady;

	void handleData();
	void gotoPlayScene();
	void createPlayer(eObjectId id);
};

#endif // !__WAITING_SCENE_H__
