#include "SpriteManager.h"
#include <iostream>
#include <string>

SpriteManager* SpriteManager::_instance = nullptr;

SpriteManager::SpriteManager()
{  
}

SpriteManager::~SpriteManager()
{
}


SpriteManager* SpriteManager::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new SpriteManager();
	}

	return _instance;
}


void SpriteManager::init()
{
	mappingNames();

	initResourceFile();
	initAnimate();
}

/// Add tất cả file plist ở đây.
void SpriteManager::initResourceFile()
{
	// Sử dụng
	// addFrameCache("tên file plist")
	// addFrameCache("tên file plist", "tên file ảnh")

	addFrameCache("yellow_tank.plist", "yellow_tank.png");

}


// Khởi tạo ứng với mỗi eAnimateId có một Animate, mỗi Animate được lấy từ SpriteFrameCache
void SpriteManager::initAnimate()
{
	// xe vàng cấp 1
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_up"].push_back(createAniamte("tank_01.png", "tank_02.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_left"].push_back(createAniamte("tank_03.png", "tank_04.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_bottom"].push_back(createAniamte("tank_05.png", "tank_06.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_right"].push_back(createAniamte("tank_07.png", "tank_08.png", NULL));
}

// Khởi tạo cho mỗi eSpriteId một Sprite
void SpriteManager::initSingleSprite()
{
	// Sprite::create("Tên file ảnh");
	// Sprite::createWithSpriteFrameName("Tên frame name trong plist")
	
}

void SpriteManager::mappingNames()
{
	// map
	_objectNames[eObjectId::YELLOW_TANK] = "yellow_tank";
	_objectNames[eObjectId::WHITE_TANK] = "white_tank";
	_objectNames[eObjectId::GREEN_TANK] = "green_tank";
}

Animate* SpriteManager::createAniamte(char* frameName, ...)
{
	va_list framenames;
	va_start(framenames, frameName);

	Vector<SpriteFrame*> spriteFrames;

	do{
		spriteFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
		frameName = va_arg(framenames, char*);
	} while (frameName != NULL);
	
	va_end(framenames);

	Animation* animation = Animation::createWithSpriteFrames(spriteFrames, DEFAULT_ANIMATION_TIME);
	auto animate = Animate::create(animation);
	animate->retain();

	return animate;
}

Animate * SpriteManager::getAnimate(const std::string &animateName, int level)
{
	if (_animates.find(animateName) == _animates.end())
		return nullptr;

	return _animates[animateName].at(level)->clone();			// should be clone, not directlyreturn nullptr;
}

Sprite* SpriteManager::getSprite(eSpriteId id)
{
	if (_sprites.find(id) == _sprites.end())
		return nullptr;

	return _sprites[id];
}

std::string SpriteManager::getObjectName(eObjectId id)
{
	return _objectNames[id];
}

void SpriteManager::addFrameCache(const std::string& plist)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist);
}

void SpriteManager::addFrameCache(const std::string& plist, const std::string& imageFile)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist, imageFile);
}

