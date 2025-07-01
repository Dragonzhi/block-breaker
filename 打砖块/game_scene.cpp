#include "brick_manager.h"
#include "camera.h"
#include "character_manager.h"
#include "collision_manager.h"
#include "game_scene.h"
#include "particle_system.h"
#include "scene_manager.h"
#include "score_manager.h"
#include "sound_manager.h"

extern int WINDOWS_WIDTH;
extern int WINDOWS_HEIGHT;

GameScene::GameScene() {
    img_background = ResourcesManager::instance()->find_image("background");
    img_purple_buff = ResourcesManager::instance()->find_image("purple_buff");
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
        int next_level = SceneManager::instance()->get_current_level() + 1;
        if (next_level > 6) next_level = 6; // 限制最大为6
        SceneManager::instance()->set_current_level(next_level);
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

    timer_undead.set_one_shot(true);
    timer_undead.set_wait_time(6.0f);
    timer_undead.set_on_timeout([this]() { 
        is_time_to_undead = false;
        });
}
GameScene::~GameScene() {}

void GameScene::on_update(float delta)  {

    Camera::instance()->on_update(delta);
    ParticleSystem::instance()->on_update(delta);

    if (is_time_to_undead) {
        timer_undead.on_update(delta);
    }

    if (is_game_overed) {
        if (!is_cheat) {
            ScoreManager::instance()->saveHighScores();
            ScoreManager::instance()->updateHighScore(SceneManager::instance()->get_current_level(), ScoreManager::instance()->getScore());
        }
         

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
        is_end_game_bg_ok = false;
        // 检查是否所有球都掉出屏幕
        auto& balls = CharacterManager::instance()->get_balls();
        bool all_balls_out = true;
        int active_balls_count = 0;

        for (Ball* ball : balls) {
            if (ball->check_enable()) {
                active_balls_count++;
                all_balls_out = false;
                break;
            }
        }
        //std::cout << "Total balls: " << balls.size() << std::endl;
        for (Ball* ball : balls) {
            if (is_undead_mode || is_time_to_undead) {
                ball->set_undead(true);
            }
            else {
                ball->set_undead(false);
            }
            //cout << "ball undead:" << ball->get_undead() << endl;
        }

        if (BrickManager::instance()->isAllBroken()) {
            is_game_overed = true;
        }

        // 如果有球掉出屏幕
        if (active_balls_count < balls.size()) {
            // 移除所有被标记为不活跃的球
            balls.erase(
                std::remove_if(balls.begin(), balls.end(),
                    [](Ball* ball) {
                        bool should_remove = !ball->check_enable();
                        if (should_remove) delete ball;
                        return should_remove;
                    }),
                balls.end()
            );

            // 如果没有球剩下，减少生命值
            if (balls.empty()) {
                Paddle* paddle = dynamic_cast<Paddle*>(CharacterManager::instance()->get_player());
                paddle->set_hp(paddle->get_hp() - 1);
                SoundManager::instance()->playSound(_T("ball_down"), false);
                if (paddle->get_hp() <= 0 ) {
                    is_game_overed = true;
                }
                else {
                    // 重新添加一个球
                    Vector2 temp_velo = { 0,0 };
                    CharacterManager::instance()->add_ball(0, 700, temp_velo, true);
                }
            }
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
        if (msg.vkcode == 0x53) {
            is_undead_mode = true;
            is_cheat = true;
            cout << "启用作弊模式，不计入成绩" << endl;
        }
        if (msg.vkcode == 0x54) {
            is_undead_mode = false;
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
    ScoreManager::instance()->loadHighScores();
    SoundManager::instance()->playSound(_T("game_bgm"), true, true);
    if (CharacterManager::instance()->get_balls().empty()) {
        Vector2 temp_velo = { 0,0 };
        CharacterManager::instance()->add_ball(0, 700, temp_velo, true);
    }
    BrickManager::instance()->fillUpperHalfRandomly_int(WINDOWS_WIDTH, WINDOWS_HEIGHT, SceneManager::instance()->get_current_level() > 6 ? 6 : SceneManager::instance()->get_current_level());
}

void GameScene::on_exit()  {
    SoundManager::instance()->stopSound(_T("game_bgm"));
    //SoundManager::instance()->stopAll();
    CharacterManager::instance()->remove_all_balls();
    rest();
    BrickManager::instance()->clearAllBricks();
    end_game_bg_position = { getwidth() / 2 - 400, getheight() };

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
    if(is_undead_mode || is_time_to_undead)
        putimage_ex(camera, img_purple_buff, &rect_dst);

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

        // 游戏结束画面评星判断
        if(CharacterManager::instance()->get_player()->get_hp() >= 1)
            putimage_alpha(camera, end_game_bg_position.x, end_game_bg_position.y, image_end_game_star_left);
        if (CharacterManager::instance()->get_player()->get_hp() >= 2)
            putimage_alpha(camera, end_game_bg_position.x, end_game_bg_position.y, image_end_game_star_right);
        if (CharacterManager::instance()->get_player()->get_hp() >= 3)
            putimage_alpha(camera, end_game_bg_position.x, end_game_bg_position.y, image_end_game_star_centre);


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

        _stprintf_s(str_cmd, _T("Highest score: %d"), ScoreManager::instance()->getHighScore(SceneManager::instance()->get_current_level()));
        int textWidth3 = textwidth(str_cmd);
        int textHeight3 = textheight(str_cmd);
        outtextxy(end_game_bg_position.x + 400 - textWidth3 / 2,
            end_game_bg_position.y + 300 + textHeight2 * 2 , // 与上一行保持10像素间距
            str_cmd);
    }
}

void GameScene::rest() {
    CharacterManager::instance()->remove_inactive_balls();

    Vector2 temp_velo = { 0,0 };
    CharacterManager::instance()->add_ball(0, 700, temp_velo, true);

    Paddle* paddle = (Paddle*)CharacterManager::instance()->get_player();
    paddle->set_hp(paddle->get_max_hp());
    paddle->to_normal();

    is_time_to_undead = false;
    is_cheat = false;
    is_undead_mode = false;

    BrickManager::instance()->rest(WINDOWS_WIDTH, WINDOWS_HEIGHT);
    is_game_overed = false;
    ScoreManager::instance()->resetScore();
}