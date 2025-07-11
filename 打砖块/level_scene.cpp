#include "level_scene.h"
#include "scene_manager.h"
#include "resources_manager.h"
#include "score_manager.h"

LevelScene::LevelScene() {
    img_background = ResourcesManager::instance()->find_image("background");

    // 创建三个关卡按钮
    button_level1 = new Button(0, 0, 128, 128);
    button_level2 = new Button(0, 0, 128, 128);
    button_level3 = new Button(0, 0, 128, 128);
    button_level4 = new Button(0, 0, 128, 128);
    button_level5 = new Button(0, 0, 128, 128);
    button_level6 = new Button(0, 0, 128, 128);
    button_level_random = new Button(0, 0, 128, 128);
    button_level_diy = new Button(0, 0, 128, 128);

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

    button_level4->set_background_image(idle_image);
    button_level4->set_click_image(press_image);
    button_level4->set_hover_image(hover_image);

    button_level5->set_background_image(idle_image);
    button_level5->set_click_image(press_image);
    button_level5->set_hover_image(hover_image);

    button_level6->set_background_image(idle_image);
    button_level6->set_click_image(press_image);
    button_level6->set_hover_image(hover_image);

    button_level_random->set_background_image(idle_image);
    button_level_random->set_click_image(press_image);
    button_level_random->set_hover_image(hover_image);

    button_level_diy->set_background_image(idle_image);
    button_level_diy->set_click_image(press_image);
    button_level_diy->set_hover_image(hover_image);


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
    button_level4->on_click([]() {         
        SceneManager::instance()->set_current_level(4);
        SceneManager::instance()->switch_to(SceneManager::SceneType::Game);  
        });
    button_level5->on_click([]() {
        SceneManager::instance()->set_current_level(5);
        SceneManager::instance()->switch_to(SceneManager::SceneType::Game);
        });
    button_level6->on_click([]() {
        SceneManager::instance()->set_current_level(6);
        SceneManager::instance()->switch_to(SceneManager::SceneType::Game);
        });
    button_level_random->on_click([]() {
        SceneManager::instance()->set_current_level(101);
        SceneManager::instance()->switch_to(SceneManager::SceneType::Game);
        });
    button_level_diy->on_click([]() {
        SceneManager::instance()->set_current_level(102);
        SceneManager::instance()->switch_to(SceneManager::SceneType::Game);
        });

    // 计算按钮的位置，使其位于场景偏上方
    int scene_width = getwidth();
    int scene_height = getheight();
    int button_width = button_level1->get_width();
    int button_height = button_level1->get_height();
    spacing = 80; 

    int total_width = 3 * button_level1->get_width() + 2 * spacing;
    int start_x = (scene_width - total_width) / 3;
    int start_y = scene_height / 4;

    button_level1->set_position(start_x, start_y);
    button_level2->set_position(start_x + button_width + spacing, start_y);
    button_level3->set_position(start_x + 2 * (button_width + spacing), start_y);
    button_level4->set_position(start_x, start_y + button_height + spacing * 2);
    button_level5->set_position(start_x + button_width + spacing, start_y + button_height + spacing * 2);
    button_level6->set_position(start_x + 2 * (button_width + spacing), start_y + button_height + spacing * 2);
    button_level_random->set_position(start_x + 3 * (button_width + spacing), start_y);
    button_level_diy->set_position(start_x + 3 * (button_width + spacing), start_y + button_height + spacing * 2);
}

LevelScene::~LevelScene() {
    delete button_level1;
    delete button_level2;
    delete button_level3;
    delete button_level4;
    delete button_level5;
    delete button_level6;
    delete button_level_random;
    delete button_level_diy;
}

void LevelScene::on_update(float delta) {
    button_level1->on_update(delta);
    button_level2->on_update(delta);
    button_level3->on_update(delta);
    button_level4->on_update(delta);
    button_level5->on_update(delta);
    button_level6->on_update(delta);
    button_level_random->on_update(delta);
    button_level_diy->on_update(delta);
}

void LevelScene::on_input(const ExMessage& msg) {
    button_level1->on_input(msg);
    button_level2->on_input(msg);
    button_level3->on_input(msg);
    button_level4->on_input(msg);
    button_level5->on_input(msg);
    button_level6->on_input(msg);
    button_level_random->on_input(msg);
    button_level_diy->on_input(msg);

    // 检查是否按下ESC键退出到菜单
    if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
        SceneManager::instance()->switch_to(SceneManager::SceneType::Menu);
    }
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
    button_level4->on_render(camera);
    button_level5->on_render(camera);
    button_level6->on_render(camera);
    button_level_random->on_render(camera);
    button_level_diy->on_render(camera);
    
    LOGFONT f;
    gettextstyle(&f);
    settextcolor(RGB(255, 255, 255));
    TCHAR str_cmd[128];

    int b_width = button_level1->get_width();
    int b_height = button_level1->get_height();

    for (int i = 1; i <= 3; i++) {
        _stprintf_s(str_cmd, _T("Level %d"), i);
        int textWidth = textwidth(str_cmd);
        int textHeight = textheight(str_cmd);
        int btn_x = button_level1->get_position().x + (i - 1) * (b_width + spacing);
        int btn_y = button_level1->get_position().y;
        outtextxy(btn_x + b_width / 2 - textWidth / 2, btn_y - textHeight * 2, str_cmd);

        int highScore = ScoreManager::instance()->getHighScore(i);
        _stprintf_s(str_cmd, _T("High: %d"), highScore);
        int scoreWidth = textwidth(str_cmd);
        outtextxy(btn_x + b_width / 2 - scoreWidth / 2, btn_y + b_height + textHeight / 2, str_cmd);
    }

    for (int i = 1; i <= 3; i++) {
        int level = i + 3;
        _stprintf_s(str_cmd, _T("Level %d"), level);
        int textWidth = textwidth(str_cmd);
        int textHeight = textheight(str_cmd);
        int btn_x = button_level4->get_position().x + (i - 1) * (b_width + spacing);
        int btn_y = button_level4->get_position().y;

        outtextxy(btn_x + b_width / 2 - textWidth / 2, btn_y - textHeight * 2, str_cmd);

        int highScore = ScoreManager::instance()->getHighScore(level);
        _stprintf_s(str_cmd, _T("High: %d"), highScore);
        int scoreWidth = textwidth(str_cmd);
        outtextxy(btn_x + b_width / 2 - scoreWidth / 2, btn_y + b_height + textHeight / 2, str_cmd);
    }

    _stprintf_s(str_cmd, _T("Random Level"));
    int textWidth = textwidth(str_cmd);
    int textHeight = textheight(str_cmd);
    int btn_x = button_level_random->get_position().x;
    int btn_y = button_level_random->get_position().y;
    outtextxy(btn_x + b_width / 2 - textWidth / 2, btn_y - textHeight * 2, str_cmd);
    int highScore = ScoreManager::instance()->getHighScore(101);
    _stprintf_s(str_cmd, _T("High: %d"), highScore);
    int scoreWidth = textwidth(str_cmd);
    outtextxy(btn_x + b_width / 2 - scoreWidth / 2, btn_y + b_height + textHeight / 2, str_cmd);

    _stprintf_s(str_cmd, _T("DIY Level"));
    textWidth = textwidth(str_cmd);
    textHeight = textheight(str_cmd);
    btn_x = button_level_diy->get_position().x;
    btn_y = button_level_diy->get_position().y;
    outtextxy(btn_x + b_width / 2 - textWidth / 2, btn_y - textHeight * 2, str_cmd);
    highScore = ScoreManager::instance()->getHighScore(102);
    _stprintf_s(str_cmd, _T("High: %d"), highScore);
    scoreWidth = textwidth(str_cmd);
    outtextxy(btn_x + b_width / 2 - scoreWidth / 2, btn_y + b_height + textHeight / 2, str_cmd);


    // 绘制总分
    int totalScore = ScoreManager::instance()->getTotalScore();
    TCHAR totalStr[64];
    _stprintf_s(totalStr, _T("Total Score: %d"), totalScore);
    int totalTextWidth = textwidth(totalStr);
    int totalTextHeight = textheight(totalStr);
    int rightMargin = 80; // 距离右边的像素
    int topMargin = getheight()/2;   // 距离顶部的像素
    outtextxy(getwidth() - totalTextWidth - rightMargin, topMargin, totalStr);

}