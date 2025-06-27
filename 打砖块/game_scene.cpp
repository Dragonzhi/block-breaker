#include "brick_manager.h"
#include "camera.h"
#include "character_manager.h"
#include "collision_manager.h"
#include "game_scene.h"
#include "particle_system.h"
#include "scene_manager.h"
#include "score_manager.h"

extern int WINDOWS_WIDTH;
extern int WINDOWS_HEIGHT;

GameScene::GameScene() {
    img_background = ResourcesManager::instance()->find_image("background");
    image_end_game_bg = ResourcesManager::instance()->find_image("end_game_background");
    image_end_game_star_left = ResourcesManager::instance()->find_image("end_game_background_star_left");
    image_end_game_star_right = ResourcesManager::instance()->find_image("end_game_background_star_right");
    image_end_game_star_centre = ResourcesManager::instance()->find_image("end_game_background_star_centre");

    button_next = new Button(0,0,128,128);
    button_next->set_background_image(ResourcesManager::instance()->find_image("end_game_background_button_Next_idle"));
    button_next->set_click_image(ResourcesManager::instance()->find_image("end_game_background_button_Next_press"));
    button_next->set_hover_image(ResourcesManager::instance()->find_image("end_game_background_button_Next_hold"));
    button_next->on_click([this]() { 
        GameScene::rest();
        SceneManager::instance()->set_current_level(SceneManager::instance()->get_current_level() + 1);
        SceneManager::instance()->switch_to(SceneManager::SceneType::Game);
        });

    button_home = new Button(0, 0, 128, 128);
    button_home->set_background_image(ResourcesManager::instance()->find_image("end_game_background_button_Home_idle"));
    button_home->set_click_image(ResourcesManager::instance()->find_image("end_game_background_button_Home_press"));
    button_home->set_hover_image(ResourcesManager::instance()->find_image("end_game_background_button_Home_hold"));
    button_home->on_click([]() { SceneManager::instance()->switch_to(SceneManager::SceneType::Menu); });

    button_rest = new Button(0, 0, 128, 128);
    button_rest->set_background_image(ResourcesManager::instance()->find_image("end_game_background_button_Rest_idle"));
    button_rest->set_click_image(ResourcesManager::instance()->find_image("end_game_background_button_Rest_press"));
    button_rest->set_hover_image(ResourcesManager::instance()->find_image("end_game_background_button_Rest_hold"));
    button_rest->on_click([]() { SceneManager::instance()->switch_to(SceneManager::SceneType::Game); });

    end_game_bg_position = { getwidth() / 2 - 400, getheight()};
}
GameScene::~GameScene() {}

void GameScene::on_update(float delta)  {

    Camera::instance()->on_update(delta);
    ParticleSystem::instance()->on_update(delta);

    if (is_game_overed) {
        if (end_game_bg_position.y >= getheight()/10) {
            end_game_bg_position.y -= 380.0f * delta;
        }
        else {
            is_end_game_bg_ok = true;
            button_home->set_position(end_game_bg_position.x + 180, end_game_bg_position.y + 470);
            button_rest->set_position(end_game_bg_position.x + 334, end_game_bg_position.y + 470);
            button_next->set_position(end_game_bg_position.x + 488, end_game_bg_position.y + 470);
            button_home->on_update(delta);
            button_rest->on_update(delta);
            button_next->on_update(delta);
        }
    }
    else {
        BrickManager::instance()->on_update(delta);
        CharacterManager::instance()->on_update(delta);
        CollisionManager::instance()->process_collide();

        if (CharacterManager::instance()->get_player()->get_hp() <= 0 || BrickManager::instance()->isAllBroken()) {
            is_game_overed = true;
            ScoreManager::instance()->saveHighScore();
        }
    }
}

void GameScene::on_input(const ExMessage& msg)  {
    if (!is_game_overed) {
        CharacterManager::instance()->on_input(msg);
        BrickManager::instance()->on_input(msg);
    }


    switch (msg.message)
    {
    case WM_KEYUP:
        if (msg.vkcode == 0x52) {
            is_debug = !is_debug;
        }
        if (msg.vkcode == 0x46) {
            BrickManager::instance()->fillWithGrid(WINDOWS_WIDTH, WINDOWS_HEIGHT, 2, 2, 1);
        }
        if (msg.vkcode == VK_ESCAPE) {
            SceneManager::instance()->switch_to(SceneManager::SceneType::Level);
            cout << "esc" << endl;
        }       
        break;

    default:
        break;
    }

    if (is_end_game_bg_ok) {
        button_home->on_input(msg);
        button_rest->on_input(msg);
        button_next->on_input(msg);
    }
}

void GameScene::on_enter()  {
    play_audio(_T("game_bgm"), true);
    BrickManager::instance()->fillUpperHalfRandomly_int(WINDOWS_WIDTH, WINDOWS_HEIGHT, SceneManager::instance()->get_current_level() > 3 ? 3 : SceneManager::instance()->get_current_level());
}

void GameScene::on_exit()  {
    stop_audio(_T("game_bgm"));
    rest();
    BrickManager::instance()->clearAllBricks();
}

void GameScene::on_render(const Camera& camera)  {
    Rect rect_dst =
    {
        (getwidth() - img_background->getwidth()) / 2,
        (getheight() - img_background->getheight()) / 2,
        img_background->getwidth(),
        img_background->getheight()
    };
    putimage_ex(camera, img_background, &rect_dst);

    BrickManager::instance()->on_render(camera);
	CharacterManager::instance()->on_render(camera);
    if(is_debug)
	    CollisionManager::instance()->on_debug_render(camera);

    ParticleSystem::instance()->on_render(camera);

    settextcolor(RGB(255, 255, 255));
    TCHAR str_cmd[128];
    _stprintf_s(str_cmd, _T("Score: %d"), ScoreManager::instance()->getScore());
    outtextxy(24, getheight() - 30, str_cmd);
    _stprintf_s(str_cmd, _T("Hp: %d"), CharacterManager::instance()->get_player()->get_hp());
    outtextxy(getwidth() - 80, getheight() - 30, str_cmd);

    if (is_game_overed) {
        render_game_overed(camera);
    }
}

void GameScene::render_game_overed(const Camera& camera) {
    putimage_alpha(camera, end_game_bg_position.x, end_game_bg_position.y, image_end_game_bg);
    if (is_end_game_bg_ok) {
        putimage_alpha(camera, end_game_bg_position.x, end_game_bg_position.y, image_end_game_star_left);
        putimage_alpha(camera, end_game_bg_position.x, end_game_bg_position.y, image_end_game_star_centre);
        putimage_alpha(camera, end_game_bg_position.x, end_game_bg_position.y, image_end_game_star_right);

        button_home->on_render(camera);
        button_rest->on_render(camera);
        button_next->on_render(camera);

        // 获取当前文本样式
        LOGFONT f;
        gettextstyle(&f); // 获取当前字体设置
        settextcolor(RGB(0, 0, 0));
        TCHAR str_cmd[128];
        _stprintf_s(str_cmd, _T("Your reward"));
        int textWidth1 = textwidth(str_cmd);  // 获取文本宽度
        int textHeight1 = textheight(str_cmd); // 获取文本高度
        outtextxy(end_game_bg_position.x + 400 - textWidth1 / 2,
            end_game_bg_position.y + 300 - textHeight1 / 2,
            str_cmd);

        _stprintf_s(str_cmd, _T("%d"), ScoreManager::instance()->getScore());
        int textWidth2 = textwidth(str_cmd);
        int textHeight2 = textheight(str_cmd);
        outtextxy(end_game_bg_position.x + 400 - textWidth2 / 2,
            end_game_bg_position.y + 300 + textHeight1, // 与上一行保持10像素间距
            str_cmd);

        _stprintf_s(str_cmd, _T("Highest score: %d"), ScoreManager::instance()->getHighScore());
        int textWidth3 = textwidth(str_cmd);
        int textHeight3 = textheight(str_cmd);
        outtextxy(end_game_bg_position.x + 400 - textWidth3 / 2,
            end_game_bg_position.y + 300 + textHeight2 * 2 , // 与上一行保持10像素间距
            str_cmd);
    }
}

void GameScene::rest() {
    Paddle* paddle = (Paddle*)CharacterManager::instance()->get_player();
    Ball* ball = (Ball*)CharacterManager::instance()->get_ball();

    paddle->set_hp(paddle->get_max_hp());
    ball->set_enable(false);
    ball->set_position(ball->get_position()-50); // 避免开局 -1hp
    BrickManager::instance()->rest(WINDOWS_WIDTH, WINDOWS_HEIGHT);
    is_game_overed = false;
    ScoreManager::instance()->resetScore();
}