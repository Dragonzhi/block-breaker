#include "menu_scene.h"
#include "game_scene.h"
#include "character_manager.h"
#include "collision_manager.h"

MenuScene::MenuScene() {
	img_background = ResourcesManager::instance()->find_image("menu_background");
}
MenuScene::~MenuScene() {}

void MenuScene::on_update(float delta) {}
void MenuScene::on_input(const ExMessage& msg) {}
void MenuScene::on_enter() {

}
void MenuScene::on_exit() {}
void MenuScene::on_render() {
    const Rect rect_dst =
    {
        (getwidth() - img_background->getwidth()) / 2,
        (getheight() - img_background->getheight()) / 2,
        img_background->getwidth(),
        img_background->getheight()
    };
    putimage_ex(img_background, &rect_dst);
}