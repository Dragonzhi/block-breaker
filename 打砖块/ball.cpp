#include "ball.h"
#include "character_manager.h"
#include "brick.h"

#include <random>

Ball::Ball() {
    //paddle = CharacterManager::instance()->get_player();
    
    position = { 0, 700 };            // ��ʼ�����λ��
    velocity = { 0, 0 };           // ��ʼ������ٶ�
    enable_through_floor = true;
    set_gravity_enabled(false);

    //hit_box->set_size({ 20, 20 });    
    //hit_box->set_layer_src(CollisionLayer::Ball);
    //hit_box->set_layer_dst(CollisionLayer::None);
    //hit_box->set_owner(this);  // ������ײ����������

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
bool Ball::check_trajectory_collision(const Vector2& start, const Vector2& end, CollisionBox* brick_box, CollisionBox::CollisionInfo& out_info) {
    // ��ȡש��ı߽�
    Vector2 brick_size = brick_box->get_size();
    Vector2 brick_pos = brick_box->get_position();

    float brick_left = brick_pos.x - brick_size.x / 2;
    float brick_right = brick_pos.x + brick_size.x / 2;
    float brick_top = brick_pos.y - brick_size.y / 2;
    float brick_bottom = brick_pos.y + brick_size.y / 2;

    // ��ȡ��İ뾶
    float radius = this->radius;

    // ��չש��߽��Կ�����İ뾶
    float expanded_left = brick_left - radius;
    float expanded_right = brick_right + radius;
    float expanded_top = brick_top - radius;
    float expanded_bottom = brick_bottom + radius;

    // ʹ�� slab �㷨�����������չ���ש�����ײ
    Vector2 ray = end - start;
    float length = ray.length();
    if (length < 0.0001f) return false; // ���Լ�С���ƶ�

    Vector2 direction = ray.normalize();
    float t_min = 0.0f;
    float t_max = length;

    // ��� X ��
    if (std::abs(direction.x) < 0.0001f) {
        // ƽ���� X ��
        if (start.x < expanded_left || start.x > expanded_right) return false;
    }
    else {
        float inv_direction = 1.0f / direction.x;
        float t1 = (expanded_left - start.x) * inv_direction;
        float t2 = (expanded_right - start.x) * inv_direction;

        if (t1 > t2) std::swap(t1, t2);
        t_min = max(t_min, t1);
        t_max = min(t_max, t2);

        if (t_min > t_max) return false;
    }

    // ��� Y ��
    if (std::abs(direction.y) < 0.0001f) {
        // ƽ���� Y ��
        if (start.y < expanded_top || start.y > expanded_bottom) return false;
    }
    else {
        float inv_direction = 1.0f / direction.y;
        float t1 = (expanded_top - start.y) * inv_direction;
        float t2 = (expanded_bottom - start.y) * inv_direction;

        if (t1 > t2) std::swap(t1, t2);
        t_min = max(t_min, t1);
        t_max = min(t_max, t2);

        if (t_min > t_max) return false;
    }

    // �������ײ��������ײ��ͷ���
    if (t_min <= length) {
        Vector2 collision_point = start + direction * t_min;

        // ��ʼ����ײ��Ϣ
        out_info.normal = Vector2(0, 0);
        out_info.penetration = radius;

        // �ҵ�����ı�
        float left_dist = std::abs(collision_point.x - brick_left);
        float right_dist = std::abs(collision_point.x - brick_right);
        float top_dist = std::abs(collision_point.y - brick_top);
        float bottom_dist = std::abs(collision_point.y - brick_bottom);

        float min_dist = min_ex({ left_dist, right_dist, top_dist, bottom_dist });

        if (min_dist == left_dist) out_info.normal = Vector2(-1, 0);
        else if (min_dist == right_dist) out_info.normal = Vector2(1, 0);
        else if (min_dist == top_dist) out_info.normal = Vector2(0, -1);
        else out_info.normal = Vector2(0, 1);

        // ���㴩͸���
        out_info.penetration = radius - min_dist;
        out_info.src = this->hurt_box;
        out_info.dst = brick_box;

        return true;
    }

    return false;
}

void Ball::handle_brick_collision(CollisionBox* brick_box, const CollisionBox::CollisionInfo& info) {
    Brick* brick = dynamic_cast<Brick*>(brick_box->get_owner());
    if (!brick) return; // ���ש����Ч��

    // �����С��͸��ֵ
    const float MIN_PENETRATION = 1.0f;
    if (info.penetration < MIN_PENETRATION) {
        return; // ����΢С��͸
    }
    // ��������ǰλ��
    Vector2 original_position = position;

    //// Ӧ��λ������
    //position += info.normal * info.penetration * 1.1f;

    //// ��ֹ�����󳬳���Ļ�ײ�
    //const float screen_bottom = getheight();
    //if (position.y > screen_bottom - radius) {
    //    position.y = screen_bottom - radius;
    //}
    //// ʹ����ײ���߽��з���
    //float dot_product = velocity.dot(info.normal);
    //Vector2 tangent_velocity = velocity - dot_product * info.normal;
    //Vector2 normal_velocity = -dot_product * info.normal;

    //// Ӧ�ûָ�ϵ��
    //const float restitution = 0.95f;
    //if (velocity.x >= 300 && velocity.x >=300) {
    //    velocity = tangent_velocity + normal_velocity * restitution;
    //}
    //else {
    //    velocity = tangent_velocity + normal_velocity;
    //}
        // λ�������������ᣩ
    Vector2 correction = info.normal * (info.penetration + 0.5f) * 0.8f;
    position += correction;

    // �ٶȷ��䣨������Ŷ���ֹ�ظ���ײ��
    float restitution = 0.9f;
    float dot = velocity.dot(info.normal);
    velocity -= (1.0f + restitution) * dot * info.normal;

    // �������Ŷ�
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.1f, 0.1f);
    velocity.x += dist(gen) * 50.0f;
    velocity.y += dist(gen) * 50.0f;

    // �������
    printf("��ײ����: (%.1f,%.1f) �� (%.1f,%.1f), ����(%.2f,%.2f), ��͸%.2f\n",
        original_position.x, original_position.y,
        position.x, position.y,
        info.normal.x, info.normal.y, info.penetration);
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

    if (velocity.x > 1000.0f) {
        velocity.x = 1000.0f;
    }
    if (velocity.y > 1000.0f) {
        velocity.y = 1000.0f;
    }

    timer_last_position.on_update(delta);

    Character::on_update(delta);
    hit_box->set_position(position);

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
        if (is_undead) {
            position.y = getheight() - 1.0f;
            reverse_y();
        }
        else {
            is_enable = false;
            velocity = { 0,0 };
        }
    }

    if (is_shot_key_down) {
        is_enable = true;
        velocity.x = -(last_position.x - position.x) * 5.0f;
        velocity.y = -speed;
        is_shot_key_down = false; 
        hurt_box->set_enabled(true);
    }

    if (!is_enable) {
        //cout << last_position.x - position.x << endl;
        CollisionBox* paddle_hurt_box = paddle->get_hurt_box();
        position.x = paddle_hurt_box->get_position().x;
        position.y = paddle_hurt_box->get_position().y - 30.0f;
        hurt_box->set_enabled(false);
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
    if (msg.message == WM_LBUTTONUP) {
        is_shot_key_down = true;
    }
}
