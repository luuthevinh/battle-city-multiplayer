#ifndef __APPEARANCE_EFFECT_H__
#define __APPEARANCE_EFFECT_H__

#include "cocos2d.h"

USING_NS_CC;

class AppearanceEffect : public Sprite
{
public:
	AppearanceEffect();
	~AppearanceEffect();

	CREATE_FUNC(AppearanceEffect);

	virtual bool init() override;

private:

};


#endif // !__APPEARANCE_EFFECT_H__
