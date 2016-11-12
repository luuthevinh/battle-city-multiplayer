#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include <vector>
#include "Scene.h"

class SceneManager
{
public:
	static SceneManager* getInstance();
	~SceneManager();

	void addScene(Scene* scene);
	void removeScene();
	void replaceScene(Scene* scene);

	void update(float dt);
	void destroy();

	Scene* getCurrentScene();

private:
	static SceneManager* _instance;
	SceneManager();

	std::vector<Scene*> _scenes;
};

#endif // !__SCENE_MANAGER_H__
