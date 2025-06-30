#include "paddle.h"
#include "resources_manager.h"
#include "ball.h"
#include "sound_manager.h"

Paddle::Paddle() {
    is_facing_left = false;
    position = { 250, getheight() - 100.0f};
    hp = 3;
    logic_height = 30;
    enable_through_floor = true;
    set_gravity_enabled(false);

    hurt_box->set_owner(this);
    hurt_box->set_size({ 100,5 });
    hurt_box->set_layer_src(CollisionLayer::Paddle);
    hurt_box->set_layer_dst(CollisionLayer::Ball);
    hurt_box->set_on_collide([this](CollisionBox* src, CollisionBox* dst, const CollisionBox::CollisionInfo& info) {
        if (src && src->get_layer_src() == CollisionLayer::Ball) {
            Ball* ball = dynamic_cast<Ball*>(src->get_owner());
            if (ball) {
                ball->handle_paddle_collision(this->hurt_box);
            }
            SoundManager::instance()->playSound(_T("ball_paddle"), false);
            is_shaking = true;
        }
        });

    paddle_animation.set_interval(0.1f);
    paddle_animation.set_loop(true);
    paddle_animation.set_anchor_mode(Animation::AnchorMode::BottomCentered);
    paddle_animation.add_frame(ResourcesManager::instance()->find_image("paddle_blue_right"), 1);

    paddle_big_animation.set_interval(0.1f);
    paddle_big_animation.set_loop(true);
    paddle_big_animation.set_anchor_mode(Animation::AnchorMode::BottomCentered);
    paddle_big_animation.add_frame(ResourcesManager::instance()->find_image("paddle_big"), 1);

    current_anim = &paddle_animation;

    timer_shaking.set_one_shot(true);
    timer_shaking.set_wait_time(0.1f);
    timer_shaking.set_on_timeout([this]() {is_shaking = false; });

    timer_big.set_one_shot(true);
    timer_big.set_wait_time(8.0f);
    timer_big.set_on_timeout([this]() {
        to_normal();
        });

    timer_toggle.set_one_shot(false);
    timer_toggle.set_wait_time(0.05f);
    timer_toggle.set_on_timeout([this]() {
        current_anim = !for_toggle ? &paddle_big_animation : &paddle_animation;
        for_toggle = !for_toggle;
        });
}

Paddle::~Paddle() {

}

void Paddle::on_input(const ExMessage& msg) {
    if (hp <= 0) return;

    switch (msg.message)
    {
    case WM_MOUSEMOVE:
        // 记录鼠标的 x 坐标
        mouse_x = msg.x;
        break;
    default:
        break;
    }
}

void Paddle::on_update(float delta) {
    if (hp > 0) {
        // 根据鼠标的 x 坐标更新 Paddle 的 position.x
        position.x = mouse_x;

        // 限制 Paddle 的位置在屏幕范围内
        if (position.x < 0) {
            position.x = 0;
        }
        else if (position.x >= getwidth()) {
            position.x = getwidth();
        }
    }

    if (is_big) {
        timer_big.on_update(delta);
        if (timer_big.get_pass_time() < 1.0f && timer_big.get_pass_time() > 0.1f) {
            timer_toggle.on_update(delta);
        }
    }

    if (is_shaking) {
        timer_shaking.on_update(delta);
        shake();
    }
    else {
        current_anim->set_position(position);
    }
    current_anim->on_update(delta);

    Character::on_update(delta);
}

void Paddle::on_render(const Camera& camera) {
    // 渲染动画
    current_anim->on_render(camera);

}