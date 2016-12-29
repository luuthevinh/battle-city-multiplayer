#include "OverScene.h"
#include "..\Base\SceneManager.h"
#include "..\Scene\PlayScene.h"

OverScene::OverScene()
{
}

OverScene::~OverScene()
{
}

bool OverScene::init()
{
	return true;
}

void OverScene::update(float dt)
{
}

void OverScene::destroy()
{
}

void OverScene::handleData(Serializable * object)
{
	switch (object->getType())
	{
	case INTEGER:
	{
		auto packet = (IntegerPacket*)object;

		switch (packet->integerType)
		{
		case IntegerPacket::BEGIN_PLAY:
		{
			SceneManager::getInstance()->replaceScene(new PlayScene());

			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}
