#include "AppearanceEffect.h"
#include "Base\SpriteManager.h"

AppearanceEffect::AppearanceEffect()
{
}

AppearanceEffect::~AppearanceEffect()
{
}

bool AppearanceEffect::init()
{
	if (!this->initWithSpriteFrameName("effect_appearance_01.png"))
	{
		return false;
	}

	auto animation = SpriteManager::getInstance()->getAnimate("appearance");
	this->runAction(Sequence::createWithTwoActions(Repeat::create(animation, 2), RemoveSelf::create()));

	return true;
}
