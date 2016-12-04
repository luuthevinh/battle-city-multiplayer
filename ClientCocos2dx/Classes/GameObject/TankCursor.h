#ifndef __TANK_CURSOR_H__
#define __TANK_CURSOR_H__

#include "cocos2d.h"
#include "Base\Definitions.h"

USING_NS_CC;

class TankCursor : public Sprite
{
public:
	CREATE_FUNC(TankCursor);

	TankCursor();
	~TankCursor();

	virtual bool init() override;
	virtual void update(float dt) override;

	void addPosition(int index, const Vec2& position);

	void onKeyPressed(EventKeyboard::KeyCode keycode, Event* e);

	int getCurrentIndex();

private:
	std::map<eObjectId, Animate*> _animations;
	std::map<int, Vec2> _indexPositions;

	int _currentIndex;

	void updatePositionWithIndex();
};

#endif // !__TANK_CURSOR_H__
