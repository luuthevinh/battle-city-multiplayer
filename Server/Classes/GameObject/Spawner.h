#ifndef __SPAWNER_H__
#define __SPAWNER_H__

#include "..\Definitions.h"

class PlayScene;
class GameObject;

class Spawner
{
public:
	Spawner(eObjectId type, PlayScene* parent);
	~Spawner();

	bool init();
	void update(float dt);

	void setTime(float time);
	float getTime();

	// số lần tạo object, -1 là vô hạn
	void setTotalObjects(int number);
	int getTotalObjects();

private:
	eObjectId _objectType;

	PlayScene* _parentScene;

	int _total;
	int _counter;

	float _timeSpan;
	float _timer;

	GameObject* createObject();
	void createObjectAndAddToParent();
};


#endif // !__SPAWNER_H___
