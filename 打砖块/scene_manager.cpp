#include "scene_manager.h"
#include "sound_manager.h"

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
    //switch (msg.message)
    //{
    //case WM_KEYUP:
    //    switch (type)
    //    {
    //    case SceneManager::SceneType::Menu:
    //        /*switch_to(SceneManager::SceneType::Game);*/
    //        break;
    //    case SceneManager::SceneType::Game:
    //        // "Q"
    //        if (msg.vkcode == 0x51) {
    //            switch_to(SceneManager::SceneType::Menu);
    //        }
    //        break;
    //    case SceneManager::SceneType::Level:

    //        break;
    //    default:
    //        break;
    //    }
    //    break;
    //default:
    //    break;
    //}
}

void SceneManager::on_update(float delta)
{
    current_scene->on_update(delta);
    SoundManager::instance()->update();
}

void SceneManager::on_render(const Camera& camera)
{
    current_scene->on_render(camera);
}

SceneManager::SceneManager()
{
    game_scene = new GameScene();
    menu_scene = new MenuScene();
    level_scene = new LevelScene();
}

SceneManager::~SceneManager()
{
    delete game_scene;
    delete menu_scene;
    delete level_scene;
}