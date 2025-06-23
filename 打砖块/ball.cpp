#include "ball.h"
#include "character_manager.h"

Ball::Ball() {
    position = { 300, 300 };            // ��ʼ�����λ��
    velocity = { speed/2, -speed };           // ��ʼ������ٶ�
    hurt_box = CollisionManager::instance()->create_collision_box();
    hurt_box->set_size({ 20, 20 });     // ������ײ�д�С
    hurt_box->set_layer_src(CollisionLayer::Ball);
    hurt_box->set_layer_dst(CollisionLayer::Paddle);
    hurt_box->set_on_collide([&]() {
            CollisionBox* paddle_hurt_box = paddle->get_hurt_box();
            reverse_y();
            position.y = paddle_hurt_box->get_position().y - hurt_box->get_size().y - 1.0f;
        });
    set_gravity_enabled(false);

    // ���ö�������
    ball_animation.set_interval(0.1f); // ֡���ʱ��
    ball_animation.set_loop(true); // ѭ������
    ball_animation.set_anchor_mode(Animation::AnchorMode::Centered);

    // ��Ӷ���֡
    ball_animation.add_frame(ResourcesManager::instance()->find_image("ball"), 1);
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
    /*if (paddle) {
        
        if (hurt_box->get_position().x + hurt_box->get_size().x >= paddle_hurt_box->get_position().x &&
            hurt_box->get_position().x <= paddle_hurt_box->get_position().x + paddle_hurt_box->get_size().x &&
            hurt_box->get_position().y + hurt_box->get_size().y >= paddle_hurt_box->get_position().y &&
            hurt_box->get_position().y <= paddle_hurt_box->get_position().y + paddle_hurt_box->get_size().y) {
            
            
        }
    }*/

    ball_animation.on_update(delta);
    ball_animation.set_position(position);
}

void Ball::on_render() {
    /*setfillcolor(RGB(255,100,100));
    solidcircle((int)position.x, (int)position.y, 10);*/
    ball_animation.on_render();
}

void Ball::on_input(const ExMessage& msg){}
