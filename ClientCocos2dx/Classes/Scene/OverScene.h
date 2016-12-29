#ifndef __OVER_SCENE_H__
#define __OVER_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class OverScene : public cocos2d::Layer
{
public:
	OverScene();
	~OverScene();

	static Scene* createScene();

	CREATE_FUNC(OverScene);

	virtual bool init() override;
	virtual void update(float dt);

	void onPlayAgainTouch(Ref* node);
	void onMenuTouch(Ref* node);
	void handleData();

private:

};

#endif // !__OVER_SCENE_H__
