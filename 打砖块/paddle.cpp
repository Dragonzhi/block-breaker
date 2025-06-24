#include "paddle.h"
#include "resources_manager.h"

Paddle::Paddle() {
    is_facing_left = false;
    position = { 250, 600};

    logic_height = 25;
    enable_through_floor = true;
    set_gravity_enabled(false);

    hurt_box->set_size({ 100,20 });
    hurt_box->set_layer_src(CollisionLayer::Paddle);
    hurt_box->set_layer_dst(CollisionLayer::Ball);

    // 初始化动画
    paddle_animation.set_interval(0.1f);
    paddle_animation.set_loop(true);
    paddle_animation.set_anchor_mode(Animation::AnchorMode::BottomCentered);
    paddle_animation.add_frame(ResourcesManager::instance()->find_image("paddle_blue_right"), 1);
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

    // 更新动画位置
    paddle_animation.set_position(position);
    paddle_animation.on_update(delta);

    Character::on_update(delta);
}

void Paddle::on_render() {
    // 渲染动画
    paddle_animation.on_render();
}