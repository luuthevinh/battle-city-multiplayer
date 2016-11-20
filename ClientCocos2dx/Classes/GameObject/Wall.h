#ifndef __WALL_H__
#define __WALL_H__

#include "Base\GameObject.h"

class Wall : public GameObject
{
public:
	Wall(eObjectId id);
	Wall(Buffer &buffer);

	~Wall();

	static Wall* createWithType(eObjectId id);
	static Wall* createWithBuffer(Buffer &buffer);

	virtual bool init() override;

	virtual void updateWithStatus(eStatus status) override;

private:

};

#endif // !__WALL_H__
