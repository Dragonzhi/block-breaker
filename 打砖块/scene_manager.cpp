#include "scene_manager.h"

SceneManager* SceneManager::manager = nullptr;

SceneManager* SceneManager::instance()
{
    if (!manager)
        manager = new SceneManager();
    return manager;
}

void SceneManager::on_input(const ExMessage& msg)
{
    current_scene->on_input(msg);
}

void SceneManager::on_update(float delta)
{
    current_scene->on_update(delta);
}

void SceneManager::on_render()
{
    current_scene->on_render();
}

SceneManager::SceneManager()
{
    game_scene = new GameScene();
    menu_scene = new MenuScene();
}

SceneManager::~SceneManager()
{
    delete game_scene;
    delete menu_scene;
}