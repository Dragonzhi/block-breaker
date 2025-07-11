#pragma once

#include <graphics.h>
#include "scene.h"
#include "game_scene.h"
#include "menu_scene.h"
#include "level_scene.h"

class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game,
		Level
	};
public:
	static SceneManager* instance();
	void on_input(const ExMessage& msg);
	void on_update(float delta);
	void on_render(const Camera& camera);

	void set_current_scene(SceneType type) {
		switch (type)
		{
		case SceneType::Menu:
			current_scene = menu_scene;
			break;
		case SceneType::Game:
			current_scene = game_scene;
			break;
		case SceneType::Level:
			current_scene = level_scene;
			break;
		default:
			break;
		}
		this->type = type;
		current_scene->on_enter();
	}

	Scene* get_current_scene() const {
		return current_scene;
	}

	void switch_to(SceneType type) {
		current_scene->on_exit();
		set_current_scene(type);
	}

	int get_current_level() const {
		return current_level;
	}

	void set_current_level(int val) {
		current_level = val;
	}

private:
	static SceneManager* manager;
	Scene* current_scene = nullptr;
	Scene* game_scene;
	Scene* menu_scene;
	Scene* level_scene;
	SceneType type;

	int current_level = 1;

private:
	SceneManager();
	~SceneManager();
};