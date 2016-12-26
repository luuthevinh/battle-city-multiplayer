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

	initSingleSprite();
}

/// Add tất cả file plist ở đây.
void SpriteManager::initResourceFile()
{
	// Sử dụng
	// addFrameCache("tên file plist")
	// addFrameCache("tên file plist", "tên file ảnh")

	addFrameCache("spritesheet.plist", "spritesheet.png");
}


// Khởi tạo ứng với mỗi eAnimateId có một Animate, mỗi Animate được lấy từ SpriteFrameCache
void SpriteManager::initAnimate()
{
	// xe vàng cấp 1
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_default_" + "_up"].push_back(createAniamte("tank_01.png", "tank_02.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_default_" + "_left"].push_back(createAniamte("tank_03.png", "tank_04.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_default_" + "_down"].push_back(createAniamte("tank_05.png", "tank_06.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_default_" + "_right"].push_back(createAniamte("tank_07.png", "tank_08.png", NULL));

	// xe vàng cấp 2
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_second_" + "_up"].push_back(createAniamte("tank_09.png", "tank_10.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_second_" + "_left"].push_back(createAniamte("tank_11.png", "tank_12.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_second_" + "_down"].push_back(createAniamte("tank_13.png", "tank_14.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_second_" + "_right"].push_back(createAniamte("tank_15.png", "tank_16.png", NULL));

	// xe vàng cấp 3
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_third_" + "_up"].push_back(createAniamte("tank_17.png", "tank_18.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_third_" + "_left"].push_back(createAniamte("tank_19.png", "tank_20.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_third_" + "_down"].push_back(createAniamte("tank_21.png", "tank_22.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_third_" + "_right"].push_back(createAniamte("tank_23.png", "tank_24.png", NULL));

	// xe vàng cấp 4
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_fourth_" + "_up"].push_back(createAniamte("tank_25.png", "tank_26.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_fourth_" + "_left"].push_back(createAniamte("tank_27.png", "tank_28.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_fourth_" + "_down"].push_back(createAniamte("tank_29.png", "tank_30.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_fourth_" + "_right"].push_back(createAniamte("tank_31.png", "tank_32.png", NULL));

	// xe vàng basic
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_basic_" + "_up"].push_back(createAniamte("tank_33.png", "tank_34.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_basic_" + "_left"].push_back(createAniamte("tank_35.png", "tank_36.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_basic_" + "_down"].push_back(createAniamte("tank_37.png", "tank_38.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_basic_" + "_right"].push_back(createAniamte("tank_39.png", "tank_40.png", NULL));

	// xe vàng fast
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_fast_" + "_up"].push_back(createAniamte("tank_41.png", "tank_42.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_fast_" + "_left"].push_back(createAniamte("tank_43.png", "tank_44.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_fast_" + "_down"].push_back(createAniamte("tank_45.png", "tank_46.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_fast_" + "_right"].push_back(createAniamte("tank_47.png", "tank_48.png", NULL));

	// xe vàng power
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_power_" + "_up"].push_back(createAniamte("tank_49.png", "tank_50.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_power_" + "_left"].push_back(createAniamte("tank_51.png", "tank_52.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_power_" + "_down"].push_back(createAniamte("tank_53.png", "tank_54.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_power_" + "_right"].push_back(createAniamte("tank_55.png", "tank_56.png", NULL));

	// xe vàng armor
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_armor_" + "_up"].push_back(createAniamte("tank_57.png", "tank_58.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_armor_" + "_left"].push_back(createAniamte("tank_59.png", "tank_60.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_armor_" + "_down"].push_back(createAniamte("tank_61.png", "tank_62.png", NULL));
	this->_animates[_objectNames[eObjectId::YELLOW_TANK] + "_armor_" + "_right"].push_back(createAniamte("tank_63.png", "tank_64.png", NULL));

	// xe xanh cấp 1
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_default_" + "_up"].push_back(createAniamte("tank_green_01.png", "tank_green_02.png", NULL));
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_default_" + "_left"].push_back(createAniamte("tank_green_03.png", "tank_green_04.png", NULL));
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_default_" + "_down"].push_back(createAniamte("tank_green_05.png", "tank_green_06.png", NULL));
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_default_" + "_right"].push_back(createAniamte("tank_green_07.png", "tank_green_08.png", NULL));

	// xe xanh cấp 2
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_second_" + "_up"].push_back(createAniamte("tank_green_09.png", "tank_green_10.png", NULL));
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_second_" + "_left"].push_back(createAniamte("tank_green_11.png", "tank_green_12.png", NULL));
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_second_" + "_down"].push_back(createAniamte("tank_green_13.png", "tank_green_14.png", NULL));
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_second_" + "_right"].push_back(createAniamte("tank_green_15.png", "tank_green_16.png", NULL));

	// xe xanh cấp 3
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_third_" + "_up"].push_back(createAniamte("tank_green_17.png", "tank_green_18.png", NULL));
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_third_" + "_left"].push_back(createAniamte("tank_green_19.png", "tank_green_20.png", NULL));
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_third_" + "_down"].push_back(createAniamte("tank_green_21.png", "tank_green_22.png", NULL));
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_third_" + "_right"].push_back(createAniamte("tank_green_23.png", "tank_green_24.png", NULL));

	// xe xanh cấp 4
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_fourth_" + "_up"].push_back(createAniamte("tank_green_25.png", "tank_green_26.png", NULL));
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_fourth_" + "_left"].push_back(createAniamte("tank_green_27.png", "tank_green_28.png", NULL));
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_fourth_" + "_down"].push_back(createAniamte("tank_green_29.png", "tank_green_30.png", NULL));
	this->_animates[_objectNames[eObjectId::GREEN_TANK] + "_fourth_" + "_right"].push_back(createAniamte("tank_green_31.png", "tank_green_32.png", NULL));

	// xe trắng basic
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_basic_" + "_up"].push_back(createAniamte("tank_white_01.png", "tank_white_02.png", NULL));
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_basic_" + "_left"].push_back(createAniamte("tank_white_03.png", "tank_white_04.png", NULL));
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_basic_" + "_down"].push_back(createAniamte("tank_white_05.png", "tank_white_06.png", NULL));
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_basic_" + "_right"].push_back(createAniamte("tank_white_07.png", "tank_white_08.png", NULL));

	// xe trắng fast
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_fast_" + "_up"].push_back(createAniamte("tank_white_41.png", "tank_white_42.png", NULL));
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_fast_" + "_left"].push_back(createAniamte("tank_white_43.png", "tank_white_44.png", NULL));
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_fast_" + "_down"].push_back(createAniamte("tank_white_45.png", "tank_white_46.png", NULL));
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_fast_" + "_right"].push_back(createAniamte("tank_white_47.png", "tank_white_48.png", NULL));

	// xe trắng power
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_power_" + "_up"].push_back(createAniamte("tank_white_49.png", "tank_white_40.png", NULL));
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_power_" + "_left"].push_back(createAniamte("tank_white_51.png", "tank_white_52.png", NULL));
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_power_" + "_down"].push_back(createAniamte("tank_white_53.png", "tank_white_54.png", NULL));
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_power_" + "_right"].push_back(createAniamte("tank_white_55.png", "tank_white_56.png", NULL));

	// xe trắng armor
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_armor_" + "_up"].push_back(createAniamte("tank_white_57.png", "tank_white_58.png", NULL));
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_armor_" + "_left"].push_back(createAniamte("tank_white_59.png", "tank_white_60.png", NULL));
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_armor_" + "_down"].push_back(createAniamte("tank_white_61.png", "tank_white_62.png", NULL));
	this->_animates[_objectNames[eObjectId::WHITE_TANK] + "_armor_" + "_right"].push_back(createAniamte("tank_white_63.png", "tank_white_64.png", NULL));

	// animate nổ
	this->_animates[_objectNames[eObjectId::EXPLOSION]].push_back(createAniamte(
		"explosion_00.png",
		"explosion_01.png",
		"explosion_02.png",
		NULL));

	this->_animates[_objectNames[eObjectId::EXPLOSION] + "_big"].push_back(createAniamte(
			"explosion_00.png",
			"explosion_01.png",
			"explosion_02.png",
			"explosion_03.png",
			"explosion_04.png",
			NULL));

	// nước
	this->_animates[_objectNames[eObjectId::WATER_WALL]].push_back(createAniamte("water_01.png", "water_02.png", nullptr));
}

// Khởi tạo cho mỗi eSpriteId một Sprite
void SpriteManager::initSingleSprite()
{
	// Sprite::create("Tên file ảnh");
	// Sprite::createWithSpriteFrameName("Tên frame name trong plist")
	
	//SpriteFrameCache::getInstance()->addSpriteFrame(
	//	SpriteFrame::create(_objectNames[eObjectId::BULLET] + ".png", Rect(0, 0, 8, 6)), _objectNames[eObjectId::BULLET]);

}

void SpriteManager::mappingNames()
{
	// map
	_objectNames[eObjectId::YELLOW_TANK] = "yellow_tank";
	_objectNames[eObjectId::WHITE_TANK] = "white_tank";
	_objectNames[eObjectId::GREEN_TANK] = "green_tank";

	_objectNames[eObjectId::BULLET] = "bullet.png";
	_objectNames[eObjectId::EXPLOSION] = "explosion";

	_objectNames[eObjectId::STEEL_WALL] = "steel";
	_objectNames[eObjectId::BRICK_WALL] = "brick";
	_objectNames[eObjectId::GRASS_WALL] = "grass";
	_objectNames[eObjectId::ICE_WALL] = "ice";
	_objectNames[eObjectId::WATER_WALL] = "water";
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

