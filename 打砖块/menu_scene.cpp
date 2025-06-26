#include "menu_scene.h"
#include "game_scene.h"
#include "character_manager.h"
#include "collision_manager.h"
#include "scene_manager.h"

MenuScene::MenuScene() {
	img_background = ResourcesManager::instance()->find_image("menu_background");
    button_start = new Button(0, 0, 128, 128);
    button_start->set_background_image(ResourcesManager::instance()->find_image("end_game_background_button_Next_idle"));
    button_start->set_click_image(ResourcesManager::instance()->find_image("end_game_background_button_Next_press"));
    button_start->set_hover_image(ResourcesManager::instance()->find_image("end_game_background_button_Next_hold"));
    button_start->on_click([]() { SceneManager::instance()->switch_to(SceneManager::SceneType::Level); });

    button_start->set_position(getwidth() / 2 - button_start->get_width() / 2, getheight() / 2 - button_start->get_height() / 2);
}
MenuScene::~MenuScene() {
    delete button_start;
}

void MenuScene::on_update(float delta) {
    button_start->on_update(delta);
}

void MenuScene::on_input(const ExMessage& msg) {
    button_start->on_input(msg);
}

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

    button_start->on_render();
}