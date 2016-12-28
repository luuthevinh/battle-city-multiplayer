#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include <vector>
#include "..\Base\GameObject.h"
#include "..\Base\Scene.h"
#include "..\GameObject\Spawner.h"
#include "..\Shared\AStarMap.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Inherited via Scene
	virtual bool init() override;

	virtual void update(float dt) override;

	virtual void destroy() override;

	void handleData(Serializable * object) override;

	virtual void sendInitDataTo(SOCKET socket) override;

	virtual int addPlayer(int socketIndex) override;
	
	virtual void updateMap(const tank::Point& index, int value);
	virtual void updateMap(const Vector2& position, int value);
	tank::AStarMap* getMap();

	virtual void beginGame() override;

	virtual int getRandomPositionIndex();
	virtual Vector2 getBotStartPosition(int index);

	virtual const Vector2& getPlayerStartPosition() override;

private:
	void checkCollisionObjects(float dt);
	void checkStatusObjects();
	void sendChangedObjects();
	void updateSnapshot(Serializable * object);
	void createBot();

	tank::AStarMap* _aStarMap;
	std::vector<Vector2> _beginPlayerPositions;
	std::vector<Vector2> _beginBotPositions;

	int _numberOfBotsCreated;
	Spawner* _botSpawner;
};


#endif // !__PLAY_SCENE_H__
