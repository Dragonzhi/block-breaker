#include "ball.h"
#include "character_manager.h"
#include "brick.h"
#include "sound_manager.h"
#include <random>

Ball::Ball(int x = 50, int y = 700, Vector2 velocity = {0,0}, bool is_ready = false) {
    //paddle = CharacterManager::instance()->get_player();
    
    position = { (float)x, (float)y };            // ��ʼ�����λ��
    this->velocity = velocity;           // ��ʼ������ٶ�
    is_ready_shot = is_ready;
    enable_through_floor = true;
    set_gravity_enabled(false);

    hurt_box->set_size({ 20, 20 });     // ������ײ�д�С
    hurt_box->set_layer_src(CollisionLayer::Ball);
    hurt_box->set_layer_dst(CollisionLayer::None);
    hurt_box->set_owner(this);
    hurt_box->set_type(CollisionBox::CollisionBoxType::Circle);

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

Vector2 Ball::get_reflection_vector(const Vector2& incident, const Vector2& normal) const {
    // ���乫ʽ: R = I - 2(I��N)N
    float dot = incident.dot(normal);
    return incident - 2 * dot * normal;
}

// ball.cpp
void Ball::apply_collision_physics(const Vector2& normal) {
    // 1. �ָ�ϵ�� (0.95 = ����95%������)
    const float restitution = 0.95f;
    velocity *= restitution;

    // 2. ����Ŷ� (��3�ȷ�ֹ��ѭ��)
    const float random_angle = (rand() % 6 - 3) * 0.01f;
    velocity = velocity.rotate(random_angle);

    // 3. �ٶ�����
    const float min_speed = 400.0f;
    if (velocity.length() < min_speed) {
        velocity = velocity.normalize() * min_speed;
    }
}

void Ball::print_collision_debug(const Vector2& original_pos,
    const Vector2& original_vel,
    const CollisionBox::CollisionInfo& info) {
    printf("��ײϸ��:\n");
    printf("λ��: (%.1f,%.1f)��(%.1f,%.1f)\n",
        original_pos.x, original_pos.y, position.x, position.y);
    printf("�ٶ�: (%.1f,%.1f)��(%.1f,%.1f)\n",
        original_vel.x, original_vel.y, velocity.x, velocity.y);
    printf("����: (%.2f,%.2f) ��͸: %.2f\n",
        info.normal.x, info.normal.y, info.penetration);
}

void Ball::handle_brick_collision(CollisionBox* brick_box, const CollisionBox::CollisionInfo& info) {
    Brick* brick = dynamic_cast<Brick*>(brick_box->get_owner());
    if (!brick) return;

    // ����ԭʼֵ���ڵ���
    Vector2 original_position = position;
    Vector2 original_velocity = velocity;

    // 1. λ������ (��ֹС����ש����)
    position += info.normal * info.penetration * 1.1f;

    // 2. �߽籣��
    const float screen_bottom = getheight();
    if (position.y > screen_bottom - radius) {
        position.y = screen_bottom - radius;
    }

    // 3. ʹ�÷����������㷴��
    if (velocity.dot(info.normal) > 0) {
        // ʹ��ͳһ�ķ�����㷽��
        velocity = get_reflection_vector(velocity, info.normal);

        // Ӧ���������
        apply_collision_physics(info.normal);
    }

    // �������
    print_collision_debug(original_position, original_velocity, info);
}

// ����ƽ̨��ײ
void Ball::handle_paddle_collision(CollisionBox* paddle_box) {
    // ��̬���㷴���Ƕ�
    float paddle_center = paddle_box->get_position().x;
    float relative_intersect = (position.x - paddle_center) / (paddle_box->get_size().x / 2);
    float bounce_angle = relative_intersect * 45.0f;  // ���45��

    velocity.x += speed * sin(bounce_angle * 3.14159f / 180.0f);
    velocity.y = -abs(speed * cos(bounce_angle * 3.14159f / 180.0f));  // ȷ�����Ϸ���
    printf("Ball ��ײ Paddle���Ƕ� %.1f ��\n", bounce_angle);
}

void Ball::on_update(float delta) {

    timer_last_position.on_update(delta);

    Character::on_update(delta);
    hit_box->set_position(position);

    // �ٶ�����
    const float max_speed = 1200.0f;
    if (velocity.length() > max_speed) {
        velocity = velocity.normalize() * max_speed;
    }

    // ��߽���ײ
    if (position.x - radius <= 0) {
        position.x = radius; 
        reverse_x();
        SoundManager::instance()->playSound(_T("ball_windows"), false);
    }
    // �ұ߽���ײ
    else if (position.x + radius >= getwidth()) {
        position.x = getwidth() - radius;
        reverse_x();
        SoundManager::instance()->playSound(_T("ball_windows"), false);
    }

    // �ϱ߽���ײ (��Ӵ�͸����)
    if (position.y - radius <= 0) {
        position.y = radius;
        reverse_y();
        SoundManager::instance()->playSound(_T("ball_windows"), false);
        // ��ֹ���ٴ�͸
        if (velocity.y < 0) velocity.y = -velocity.y;
    }

    if (position.y > getheight()) {  // �������Ļ�ײ�
        auto& balls = CharacterManager::instance()->get_balls();
        int activeBallCount = 0;

        if (is_undead) {
            position.y = getheight() - 1.0f;
            reverse_y();
            SoundManager::instance()->playSound(_T("ball_windows"), false);
        }
        else {
            is_enable = false;
            
        }
    }

    if (is_shot_key_down) {
        is_enable = true;
        is_ready_shot = false;
        velocity.x = -(last_position.x - position.x) * 5.0f;
        velocity.y = -speed;
        is_shot_key_down = false; 
        hurt_box->set_enabled(true);
    }

    if (is_ready_shot) {
        if (paddle->get_hp() <= 0) {
            return;
        }
        //cout << last_position.x - position.x << endl;
        CollisionBox* paddle_hurt_box = paddle->get_hurt_box();
        position.x = paddle_hurt_box->get_position().x;
        position.y = paddle_hurt_box->get_position().y - 30.0f;
        hurt_box->set_enabled(false);
    }

}

void Ball::on_render(const Camera& camera) {
    /*setfillcolor(RGB(255,100,100));
    solidcircle((int)position.x, (int)position.y, 10);*/
    Character::on_render(camera);
}

void Ball::on_input(const ExMessage& msg){
    if (!is_enable || !is_ready_shot || paddle->get_hp() <= 0) return;

    // ֻ����δ����ʱ������
    if (msg.message == WM_LBUTTONUP) {
        is_shot_key_down = true;
    }

}
