#ifndef __SINGLE_PLAYE_SCENE_H__
#define __SINGLE_PLAYE_SCENE_H__

#include "cocos2d.h"

// shared
#include "..\Server\Classes\Shared\SharedDefinitions.h"

USING_NS_CC;

class SinglePlayScene : public Layer
{
public:
	static Scene* createScene();
	CREATE_FUNC(SinglePlayScene);

	SinglePlayScene();
	~SinglePlayScene();

	virtual bool init() override;

private:
	void initWithTMX();
	void addWall(const Vec2 & position, eObjectId id);
};

#endif // !__SINGLE_PLAYE_SCENE_H__
