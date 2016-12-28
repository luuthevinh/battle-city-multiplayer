#ifndef __PLAYER_SERVER_H__
#define __PLAYER_SERVER_H__

#include "../Definitions.h"
#include "Tank.h"

class Player : public Tank
{
public:
	Player(eObjectId id, int index);
	~Player();

	int getIndex() const;

	virtual void handleData(Serializable* data) override;
	virtual void shoot() override;

	void setHost(bool value);
	bool isHost() const;

	virtual void move(eDirection direction, float dt) override;

private:
	int _index;		// index socket trong client manager
	bool _isHost;

	//void updateInput(eKeyInput input, bool start);
};

#endif // !__PLAYER_SERVER_H__
