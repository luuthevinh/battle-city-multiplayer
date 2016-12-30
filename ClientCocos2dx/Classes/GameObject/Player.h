#ifndef __PLAYER_H__
#define	__PLAYER_H__

#include "Tank.h"

class Player : public Tank
{
public:
	Player(eObjectId id);
	~Player();

	static Player* create(eObjectId id);
	static Player* createInfo(Buffer& data);
	static Player* createGameObject(GameObject* object);

	virtual bool init() override;
	virtual void update(float dt) override;

	void onKeyPressed(EventKeyboard::KeyCode keycode, Event* e);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event* e);

	virtual void reconcile(Buffer &data) override;
	void predict(float dt);

	virtual void move(eDirection direction, float dt) override;
	virtual void updateWithStatus(eStatus status) override;

	void revive();
	int getLife();

private:
	int _keyDirectionCounter;
	Vec2 _lastUpdatedPosition;
	int _life;

	virtual void updateWithCommand(CommandPacket* commad, float dt) override;
	void syncPositionWithLastUpdate(float dt);

	bool _enableSync;
	std::vector<CommandPacket> _pendingCommands;
	void addPendingCommand(CommandPacket* command);
};

#endif // !__PLAYER_H__
