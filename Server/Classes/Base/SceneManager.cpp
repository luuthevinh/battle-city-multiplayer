#include "SceneManager.h"

SceneManager* SceneManager::_instance = nullptr;

SceneManager::SceneManager()
{
}

SceneManager * SceneManager::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new SceneManager();
	}
	return _instance;
}

SceneManager::~SceneManager()
{
}

void SceneManager::addScene(Scene * scene)
{
	_scenes.push_back(scene);
	_scenes.back()->init();
}

void SceneManager::removeScene()
{
	if (!_scenes.empty())
	{
		_scenes.back()->destroy();
		delete _scenes.back();

		_scenes.pop_back();
	}
}

void SceneManager::replaceScene(Scene * scene)
{
	this->removeScene();
	this->addScene(scene);
}

void SceneManager::update(float dt)
{
	if (_scenes.empty())
		return;

	_scenes.back()->update(dt);
}

void SceneManager::destroy()
{
	while (!_scenes.empty())
	{
		_scenes.back()->destroy();
		delete _scenes.back();

		_scenes.pop_back();
	}
}

Scene * SceneManager::getCurrentScene()
{
	if(_scenes.empty())
		return nullptr;

	return _scenes.back();
}
