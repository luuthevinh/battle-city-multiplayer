#ifndef __PROTECTION_EFFECT_H__
#define __PROTECTION_EFFECT_H__

#include "cocos2d.h"

USING_NS_CC;

class ProtectionEffect : public Sprite
{
public:
	ProtectionEffect();
	~ProtectionEffect();

	CREATE_FUNC(ProtectionEffect);

	virtual bool init() override;
private:

};

#endif // !__PROTECTION_EFFECT_H__
