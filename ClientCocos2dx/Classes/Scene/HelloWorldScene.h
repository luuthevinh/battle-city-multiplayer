#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Base\ServerConnector.h"
#include "GameObject\Player.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	void update(float dt) override;

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	static HelloWorld* instance;

	ServerConnector* getConnector();

	Player* _player;


private:
	ServerConnector* _serverConnector;
};

#endif // __HELLOWORLD_SCENE_H__
