#ifndef __WAITING_SCENE_H__
#define __WAITING_SCENE_H__

#include "cocos2d.h"
#include "GameObject\TankCursor.h"
#include "ui\CocosGUI.h"

USING_NS_CC;

class Player;

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
	void nameTextFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type);
	void playBtnTouchEvent(Ref* sender, ui::Widget::TouchEventType type);
	void backBtnTouchEvent(Ref* sender, ui::Widget::TouchEventType type);
	void joinBtnTouchEvent(Ref* sender, ui::Widget::TouchEventType type);

	void decreaseBtnTouchEvent(Ref* sender, ui::Widget::TouchEventType type);
	void increaseBtnTouchEvent(Ref* sender, ui::Widget::TouchEventType type);
	void yellowBtnTouchEvent(Ref* sender, ui::Widget::TouchEventType type);
	void greenBtnTouchEvent(Ref* sender, ui::Widget::TouchEventType type);

private:
	// Layer* _playLayer;
	TankCursor* _cursor;

	// ui
	ui::TextField* _textField;
	ui::TextField* _addrTextField;
	Sprite* _pointerInput;
	Sprite* _pointerPlayerSelect;
	Label* _greenTankNumber;
	Label* _whiteTankNumber;
	Label* _yellowTankNumber;

	bool _isReady;

	void handleData();
	void gotoPlayScene();
	void createPlayer(eObjectId id);
	void updateYellowNumberText();
	void updateGreenNumberText();
	void updateWhiteNumberText();

	int _greenTankCounter;
	int _yellowTankCounter;
	int _whiteTankCounter;
	int _maxBots;

	eObjectId _playerSelected;
	void updateAndSendPlayerId(eObjectId id);
	void updateNumberOfBots(int value);

	Player* _currentPlayer;
};

#endif // !__WAITING_SCENE_H__
