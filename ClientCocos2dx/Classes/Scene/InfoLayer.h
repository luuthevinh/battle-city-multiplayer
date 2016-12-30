#ifndef __INFO_LAYER_H__
#define __INFO_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class InfoLayer : public cocos2d::LayerColor
{
public:
	InfoLayer();
	~InfoLayer();

	CREATE_FUNC(InfoLayer);

	virtual bool init() override;
	virtual void update(float dt) override;

	void addWave(int number);
	void removeWave(int number);
	int getCurrentWave();

	void setPlayerLife(int player, int life);

	void onMenuTouch(Ref* node);

private:
	int _numberOfWaveColumns;
	int _currentWave;

	Label* _playerOneLife;
	Label* _playerTwoLife;
};

#endif // !__INFO_LAYER_H__
