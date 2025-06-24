#include "ball.h"
#include "character_manager.h"

Ball::Ball() {
    //paddle = CharacterManager::instance()->get_player();
    
    position = { 0, 700 };            // ��ʼ�����λ��
    velocity = { 0, 0 };           // ��ʼ������ٶ�
    enable_through_floor = true;
    set_gravity_enabled(false);

    hit_box->set_size({ 20, 20 });    
    hit_box->set_layer_src(CollisionLayer::Ball);
    hit_box->set_layer_dst(CollisionLayer::Brick);
    hit_box->set_on_collide([this]() {
        cout << position.x << " " << position.y;
        cout << "Ball -> Brick" << endl;
        });

    hurt_box->set_size({ 20, 20 });     // ������ײ�д�С
    hurt_box->set_layer_src(CollisionLayer::Ball);
    hurt_box->set_layer_dst(CollisionLayer::Paddle);
    hurt_box->set_on_collide([this]() {
            CollisionBox* paddle_hurt_box = paddle->get_hurt_box();
            reverse_y();
            position.y = paddle_hurt_box->get_position().y - hurt_box->get_size().y - 1.0f;
        });

    radius = hurt_box->get_size().y / 2;

    // ���ö�������
    ball_animation.set_interval(0.1f); // ֡���ʱ��
    ball_animation.set_loop(true); // ѭ������
    ball_animation.set_anchor_mode(Animation::AnchorMode::Centered);

    // ��Ӷ���֡
    ball_animation.add_frame(ResourcesManager::instance()->find_image("ball"), 1);
    current_animation = &ball_animation;

    timer_last_position.set_one_shot(false);
    timer_last_position.set_wait_time(0.1f);
    timer_last_position.set_on_timeout([&]() {last_position = position; });
}

Ball::~Ball() {

}

void Ball::on_update(float delta) {

    timer_last_position.on_update(delta);

    Character::on_update(delta);

    // ��߽���ײ
    if (position.x - radius <= 0) {
        position.x = radius; 
        reverse_x();
    }
    // �ұ߽���ײ
    else if (position.x + radius >= getwidth()) {
        position.x = getwidth() - radius;
        reverse_x();
    }

    // �ϱ߽���ײ (��Ӵ�͸����)
    if (position.y - radius <= 0) {
        position.y = radius;
        reverse_y();

        // ��ֹ���ٴ�͸
        if (velocity.y < 0) velocity.y = -velocity.y;
    }
    else if (position.y > getheight()) {
        is_enable = false;
        velocity = { 0,0 };
    }

    if (is_shot_key_down) {
        is_enable = true;
        velocity.x = -(last_position.x - position.x) * 5.0f;
        velocity.y = -speed;
        is_shot_key_down = false; 
    }

    if (!is_enable) {
        //cout << last_position.x - position.x << endl;
        CollisionBox* paddle_hurt_box = paddle->get_hurt_box();
        position.x = paddle_hurt_box->get_position().x;
        position.y = paddle_hurt_box->get_position().y - 30.0f;

    }

}

void Ball::on_render() {
    /*setfillcolor(RGB(255,100,100));
    solidcircle((int)position.x, (int)position.y, 10);*/
    Character::on_render();
}

void Ball::on_input(const ExMessage& msg){
    if (is_enable) return;

    // ֻ����δ����ʱ������
    if (msg.message == WM_LBUTTONDOWN) {
        is_shot_key_down = true;
    }
}
