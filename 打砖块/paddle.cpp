#include "paddle.h"
#include "resources_manager.h"
#include "ball.h"

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
        }
        });
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
        // ��¼���� x ����
        mouse_x = msg.x;
        break;
    default:
        break;
    }
}

void Paddle::on_update(float delta) {
    if (hp > 0) {
        // �������� x ������� Paddle �� position.x
        position.x = mouse_x;

        // ���� Paddle ��λ������Ļ��Χ��
        if (position.x < 0) {
            position.x = 0;
        }
        else if (position.x >= getwidth()) {
            position.x = getwidth();
        }
    }

    // ���¶���λ��
    paddle_animation.set_position(position);
    paddle_animation.on_update(delta);

    Character::on_update(delta);
}

void Paddle::on_render(const Camera& camera) {
    // ��Ⱦ����
    paddle_animation.on_render(camera);
}