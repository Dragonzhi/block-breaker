#include "ball.h"

Ball::Ball() {
	hurt_box = CollisionManager::instance()->create_collision_box();
    position = { 300, 300 };            // ��ʼ�����λ��
    velocity = { speed/2, -speed };           // ��ʼ������ٶ�
    hurt_box->set_size({ 10, 10 });     // ������ײ�д�С
    hurt_box->set_layer_src(CollisionLayer::Ball);
    hurt_box->set_layer_dst(CollisionLayer::Paddle);
    set_gravity_enabled(false);
}

Ball::~Ball() {
	CollisionManager::instance()->destroy_collision_box(hurt_box);
}

void Ball::on_update(float delta) {
    position += velocity * delta;
    hurt_box->set_position(position);

    // ��Ļ��Ե��ײ���
    if (position.x <= 0 || position.x + hurt_box->get_size().x >= getwidth()) {
        reverse_x();
    }
    if (position.y <= 0) {
        reverse_y();
    }

    // ����Ƿ�����paddle
    if (paddle) {
        CollisionBox* paddle_hurt_box = paddle->get_hurt_box();
        if (hurt_box->get_position().x + hurt_box->get_size().x >= paddle_hurt_box->get_position().x &&
            hurt_box->get_position().x <= paddle_hurt_box->get_position().x + paddle_hurt_box->get_size().x &&
            hurt_box->get_position().y + hurt_box->get_size().y >= paddle_hurt_box->get_position().y /*&&
            hurt_box->get_position().y <= paddle_hurt_box->get_position().y + paddle_hurt_box->get_size().y*/) {
            reverse_y();
            position.y = paddle_hurt_box->get_position().y - hurt_box->get_size().y - 1.0f;
        }
    }
}

void Ball::on_render() {
    setfillcolor(YELLOW);
    solidcircle((int)position.x, (int)position.y, 5);
}

void Ball::on_input(const ExMessage& msg){}