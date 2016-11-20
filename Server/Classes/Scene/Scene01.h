#ifndef __SCENE_01_H__
#define __SCENE_01_H__

#include <vector>
#include "..\Base\GameObject.h"
#include "..\Base\Scene.h"


class Scene01 : public Scene
{
public:
	Scene01();
	~Scene01();

	// Inherited via Scene
	virtual bool init() override;

	virtual void update(float dt) override;

	virtual void destroy() override;

	void handleData(Serializable * object) override;

	virtual void sendInitDataTo(SOCKET socket) override;

private:
	void checkStatusObjects();
};


#endif // !__SCENE_01_H__
