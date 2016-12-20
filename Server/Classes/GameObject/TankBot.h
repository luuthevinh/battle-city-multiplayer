#ifndef __TANK_BOT_H__
#define __TANK_BOT_H__

#include "Tank.h"
#include "..\Shared\Map.h"

class TankBot : public Tank
{
public:
	TankBot();
	~TankBot();

	virtual bool init() override;
	virtual void update(float dt) override;

	void setMap(tank::AStarMap* map);

	virtual void onContactBegin(GameObject& object);

private:
	// ref
	tank::AStarMap* _aStarMap;

	std::queue<Vector2> _nextPostions;
	
	void findWay();
};


#endif // !__TANK_BOT_H__
