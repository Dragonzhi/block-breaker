#include "game_scene.h"
#include "character_manager.h"
#include "collision_manager.h"
#include "brick_manager.h"

GameScene::GameScene() {
    img_background = ResourcesManager::instance()->find_image("background");
    BrickManager::instance()->fillUpperHalfRandomly_int(1280, 720);
}
GameScene::~GameScene() {}

void GameScene::on_update(float delta)  {
    BrickManager::instance()->on_update(delta);
	CharacterManager::instance()->on_update(delta);
	CollisionManager::instance()->process_collide();

}

void GameScene::on_input(const ExMessage& msg)  {
	CharacterManager::instance()->on_input(msg);
    BrickManager::instance()->on_input(msg);
}

void GameScene::on_enter()  {

}

void GameScene::on_exit()  {
    
}

void GameScene::on_render()  {
    Rect rect_dst =
    {
        (getwidth() - img_background->getwidth()) / 2,
        (getheight() - img_background->getheight()) / 2,
        img_background->getwidth(),
        img_background->getheight()
    };
    putimage_ex(img_background, &rect_dst);

    BrickManager::instance()->on_render();
	CharacterManager::instance()->on_render();
	CollisionManager::instance()->on_debug_render();


}