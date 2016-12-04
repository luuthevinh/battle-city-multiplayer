#ifndef __PLAYER_H__
#define	__PLAYER_H__

#include "Tank.h"

class Player : public Tank
{
public:
	Player(eObjectId id);
	~Player();

	static Player* create(eObjectId id);

	virtual bool init() override;
	virtual void update(float dt) override;

	void onKeyPressed(EventKeyboard::KeyCode keycode, Event* e);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* e);

private:
	int _keyDirectionCounter;

};

#endif // !__PLAYER_H__
