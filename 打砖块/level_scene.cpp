#include "level_scene.h"
#include "scene_manager.h"
#include "resources_manager.h"

LevelScene::LevelScene() {
    img_background = ResourcesManager::instance()->find_image("background");

    // 创建三个关卡按钮
    button_level1 = new Button(0, 0, 128, 128);
    button_level2 = new Button(0, 0, 128, 128);
    button_level3 = new Button(0, 0, 128, 128);

    // 设置按钮的背景、悬停和点击图片
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

    // 设置按钮的点击事件
    button_level1->on_click([]() {         
        SceneManager::instance()->set_current_level(1);
        SceneManager::instance()->switch_to(SceneManager::SceneType::Game); 
        });
    button_level2->on_click([]() {         
        SceneManager::instance()->set_current_level(2);
        SceneManager::instance()->switch_to(SceneManager::SceneType::Game);  
        });
    button_level3->on_click([]() {         
        SceneManager::instance()->set_current_level(3);
        SceneManager::instance()->switch_to(SceneManager::SceneType::Game);  
        });

    // 计算按钮的位置，使其位于场景偏上方
    int scene_width = getwidth();
    int scene_height = getheight();
    int button_width = button_level1->get_width();
    int button_height = button_level1->get_height();
    spacing = 80; 

    int total_width = 3 * button_level1->get_width() + 2 * spacing;
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
    
}

void LevelScene::on_exit() {
    
}

void LevelScene::on_render(const Camera& camera) {
    const Rect rect_dst =
    {
        (getwidth() - img_background->getwidth()) / 2,
        (getheight() - img_background->getheight()) / 2,
        img_background->getwidth(),
        img_background->getheight()
    };
    putimage_ex(camera, img_background, &rect_dst);

    button_level1->on_render(camera);
    button_level2->on_render(camera);
    button_level3->on_render(camera);

    LOGFONT f;
    gettextstyle(&f);
    settextcolor(RGB(255, 255, 255));
    TCHAR str_cmd[128];
    for (int i = 1; i <= 3; i++) {
        _stprintf_s(str_cmd, _T("Level %d"), i);
        int textWidth = textwidth(str_cmd);
        int textHeight = textheight(str_cmd);

        int b_width = button_level1->get_width();
        int b_height = button_level1->get_height();

        outtextxy((button_level1->get_position().x + b_width / 2 - textWidth / 2) + (i-1) * (b_width + spacing), button_level1->get_position().y - textHeight * 2,
            str_cmd);
    }
}