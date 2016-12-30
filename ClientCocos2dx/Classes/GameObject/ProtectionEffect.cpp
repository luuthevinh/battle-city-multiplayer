#include "ProtectionEffect.h"
#include "Base\SpriteManager.h"

ProtectionEffect::ProtectionEffect()
{
}

ProtectionEffect::~ProtectionEffect()
{
}

bool ProtectionEffect::init()
{
	if (!this->initWithSpriteFrameName("effect_protection_01.png"))
	{
		return false;
	}

	auto animation = SpriteManager::getInstance()->getAnimate("protection");
	this->runAction(RepeatForever::create(animation));
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), RemoveSelf::create()));

	return true;
}
