#include "level_scene.h"
#include "scene_manager.h"
#include "resources_manager.h"

LevelScene::LevelScene() {
    img_background = ResourcesManager::instance()->find_image("background");

    // ���������ؿ���ť
    button_level1 = new Button(0, 0, 128, 128);
    button_level2 = new Button(0, 0, 128, 128);
    button_level3 = new Button(0, 0, 128, 128);

    // ���ð�ť�ı�������ͣ�͵��ͼƬ
    IMAGE* idle_image = ResourcesManager::instance()->find_image("end_game_background_button_Next_idle");
    IMAGE* press_image = ResourcesManager::instance()->find_image("end_game_background_button_Next_press");
    IMAGE* hover_image = ResourcesManager::instance()->find_image("end_game_background_button_Next_hold");

    button_level1->set_background_image(idle_image);
    button_level1->set_click_image(press_image);
    button_level1->set_hover_image(hover_image);

    button_level2->set_background_image(idle_image);
    button_level2->set_click_image(press_image);
    button_level2->set_hover_image(hover_image);

    button_level3->set_background_image(idle_image);
    button_level3->set_click_image(press_image);
    button_level3->set_hover_image(hover_image);

    // ���ð�ť�ĵ���¼�
    button_level1->on_click([]() { std::cout << "Level 1 clicked!" << std::endl; });
    button_level2->on_click([]() { std::cout << "Level 2 clicked!" << std::endl; });
    button_level3->on_click([]() { std::cout << "Level 3 clicked!" << std::endl; });

    // ���㰴ť��λ�ã�ʹ��λ�ڳ���ƫ�Ϸ�
    int scene_width = getwidth();
    int scene_height = getheight();
    int button_width = button_level1->get_width();
    int button_height = button_level1->get_height();
    int spacing = 20; // ��ť֮��ļ��

    int total_width = 3 * button_width + 2 * spacing;
    int start_x = (scene_width - total_width) / 2;
    int start_y = scene_height / 4;

    button_level1->set_position(start_x, start_y);
    button_level2->set_position(start_x + button_width + spacing, start_y);
    button_level3->set_position(start_x + 2 * (button_width + spacing), start_y);
}

LevelScene::~LevelScene() {
    delete button_level1;
    delete button_level2;
    delete button_level3;
}

void LevelScene::on_update(float delta) {
    button_level1->on_update(delta);
    button_level2->on_update(delta);
    button_level3->on_update(delta);
}

void LevelScene::on_input(const ExMessage& msg) {
    button_level1->on_input(msg);
    button_level2->on_input(msg);
    button_level3->on_input(msg);
}

void LevelScene::on_enter() {
    // ��������ʱ�Ĳ������ɸ����������
}

void LevelScene::on_exit() {
    // �����˳�ʱ�Ĳ������ɸ����������
}

void LevelScene::on_render() {
    const Rect rect_dst =
    {
        (getwidth() - img_background->getwidth()) / 2,
        (getheight() - img_background->getheight()) / 2,
        img_background->getwidth(),
        img_background->getheight()
    };
    putimage_ex(img_background, &rect_dst);

    button_level1->on_render();
    button_level2->on_render();
    button_level3->on_render();
}