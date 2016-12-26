#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class MenuScene : public Layer 
{
public:
	MenuScene();
	~MenuScene();

	static Scene* createScene();

	CREATE_FUNC(MenuScene);

	virtual bool init() override;

	void onKeyPressed(EventKeyboard::KeyCode keycode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

	void onSinglePlayerTouch(Ref* node);
	void onMultiPlayerTouch(Ref* node);

private:

};

#endif // !__MENU_SCENE_H__
