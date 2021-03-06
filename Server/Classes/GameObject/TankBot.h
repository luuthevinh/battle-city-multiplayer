#ifndef __TANK_BOT_H__
#define __TANK_BOT_H__

#include "Tank.h"
#include "..\Shared\AStarMap.h"

USING_NS_TANK

class TankBot : public Tank
{
public:
	enum eDecision
	{
		SHOOT,
		MOVE_NEXT,
		FIND_NEW_WAY,
		STANDING
	};

	TankBot();
	~TankBot();

	virtual bool init() override;
	virtual void update(float dt) override;

	void setMap(tank::AStarMap* map);

	virtual void onContactBegin(GameObject& object);
	virtual void checkCollision(GameObject& other, float dt) override;

	Point getRandomNextPostion();

private:
	// ref
	tank::AStarMap* _aStarMap;
	bool _canContactBegin;

	std::deque<Vector2> _nextPostions;
	
	void findNewWay();
	void findNewWayWithTempObstacle(const tank::Point& index);

	void moveNext(float dt);
	void addCommand(eKeyInput input, bool begin = true);

	Vector2 _oldPositon;
	float _activeTimer;

	void countingToActive(float dt);

	eDecision think();
};


#endif // !__TANK_BOT_H__
