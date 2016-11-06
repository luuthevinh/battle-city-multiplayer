#include "Explosion.h"
#include "Base\SpriteManager.h"

Explosion::Explosion() : GameObject(eObjectId::EXPLOSION)
{
}

Explosion::~Explosion()
{
}

Explosion * Explosion::create(bool big)
{
	Explosion* explosion = new(nothrow) Explosion();

	explosion->setBig(big);

	if (explosion->init())
	{
		explosion->autorelease();
		return explosion;
	}

	CC_SAFE_DELETE(explosion);
	return nullptr;
}

bool Explosion::init()
{
	this->setName(SpriteManager::getInstance()->getObjectName(eObjectId::EXPLOSION));

	_sprite = Sprite::create();
	this->addChild(_sprite);

	if (_isBig)
	{
		auto explosion = SpriteManager::getInstance()->getAnimate(this->getName() + "_big");
		_sprite->runAction(explosion);
		this->scheduleOnce(schedule_selector(Explosion::removeSelf), DEFAULT_ANIMATION_TIME * 4);
	}
	else
	{
		auto explosion = SpriteManager::getInstance()->getAnimate(this->getName());
		_sprite->runAction(explosion);
		this->scheduleOnce(schedule_selector(Explosion::removeSelf), DEFAULT_ANIMATION_TIME * 3);
	}

	return true;
}

void Explosion::removeSelf(float dt)
{
	this->removeFromParentAndCleanup(true);
}

void Explosion::setBig(bool big)
{
	_isBig = big;
}
