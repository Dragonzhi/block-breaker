#pragma once

#include <graphics.h>
#include "scene.h"
#include "game_scene.h"
#include "menu_scene.h"

class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game,
	};
public:
	static SceneManager* instance();
	void on_input(const ExMessage& msg);
	void on_update(float delta);
	void on_render();

	void set_current_scene(SceneType type) {
		switch (type)
		{
		case SceneManager::SceneType::Menu:
			current_scene = menu_scene;
			break;
		case SceneManager::SceneType::Game:
			current_scene = game_scene;
			break;
		default:
			break;
		}
		current_scene->on_enter();
	}

	void switch_to(SceneType type) {
		current_scene->on_exit();
		set_current_scene(type);
	}

private:
	static SceneManager* manager;
	Scene* current_scene = nullptr;
	Scene* game_scene;
	Scene* menu_scene;
private:
	SceneManager();
	~SceneManager();
};