#ifndef __PLAYER_SERVER_H__
#define __PLAYER_SERVER_H__

#include "../Definitions.h"
#include "Tank.h"

class Player : public Tank
{
public:
	Player(eObjectId id, int index);
	~Player();

	int getIndex();

	void shoot();
	void updateInput(eKeyInput input, bool start);

private:
	int _index;		// index socket trong client manager
};

#endif // !__PLAYER_SERVER_H__
