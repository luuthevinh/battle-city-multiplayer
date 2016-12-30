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

	void setCurrent(int number);
	int getCurrent();

	void setCurrentMax(int number);

	eObjectId getObjectType();
	
	int getWave();
	void nextWave();

private:
	eObjectId _objectType;

	PlayScene* _parentScene;

	int _total;
	int _counter;

	int _currentMax;
	int _currentCounter;
	int _waveCounter;
	bool _canSpawn;

	float _timeSpan;
	float _timer;

	GameObject* createObject();
	void createObjectAndAddToParent();
	eTankLevel getTankLevel();
};


#endif // !__SPAWNER_H___
