#include "ball.h"
#include "character_manager.h"
#include "brick.h"

Ball::Ball() {
    //paddle = CharacterManager::instance()->get_player();
    
    position = { 0, 700 };            // ��ʼ�����λ��
    velocity = { 0, 0 };           // ��ʼ������ٶ�
    enable_through_floor = true;
    set_gravity_enabled(false);

    hit_box->set_size({ 20, 20 });    
    hit_box->set_layer_src(CollisionLayer::Ball);
    hit_box->set_layer_dst(CollisionLayer::None);

    hurt_box->set_size({ 20, 20 });     // ������ײ�д�С
    hurt_box->set_layer_src(CollisionLayer::Ball);
    hurt_box->set_layer_dst(CollisionLayer::None);

    hit_box->set_owner(this);  // ������ײ����������
    hurt_box->set_owner(this);

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


// ����ש����ײ
void Ball::handle_brick_collision(CollisionBox* brick_box) {
    printf("Ball����ש����ײ������(%.1f,%.1f)\n", position.x, position.y);
    // ��ȡש�龫ȷ��Ϣ
    Brick* brick = dynamic_cast<Brick*>(brick_box->get_owner());
    if (!brick) return;

    Vector2 brick_center = brick->get_position();
    Vector2 brick_size = brick_box->get_size();

    // ������İ뾶����������Բ�Σ�
    float ball_radius = hit_box->get_size().x / 2.0f;

    // 1. ����AABB��Բ�ε���ײ
    Vector2 closest_point;
    closest_point.x =  max(brick_center.x - brick_size.x / 2,
         min(position.x, brick_center.x + brick_size.x / 2));
    closest_point.y =  max(brick_center.y - brick_size.y / 2,
         min(position.y, brick_center.y + brick_size.y / 2));

    // 2. �������ĵ�����������
    Vector2 collision_vector = position - closest_point;
    float distance = collision_vector.length();

    // 3. ����Ƿ�����ײ
    if (distance > ball_radius) {
        return; // û����ײ
    }

    // 4. ������Сλ������(MTV)
    Vector2 collision_normal = collision_vector.normalize();
    float penetration_depth = ball_radius - distance;

    // 5. λ�������������Ƴ���ײ����
    position += collision_normal * penetration_depth * 1.1f;

    // 6. ������ײ���ߣ�������ײ�棩
    Vector2 actual_normal = { 0, 0 };

    // ȷ����ײ��������һ��
    Vector2 to_center = brick_center - position;
    float abs_x = abs(to_center.x);
    float abs_y = abs(to_center.y);

    // �����ص���
    float overlap_x = ball_radius + brick_size.x / 2 - abs_x;
    float overlap_y = ball_radius + brick_size.y / 2 - abs_y;

    // ȷ����С�ص�������������ײ���ߣ�
    if (overlap_x < overlap_y) {
        actual_normal.x = (to_center.x < 0) ? -1 : 1;
    }
    else {
        actual_normal.y = (to_center.y < 0) ? -1 : 1;
    }

    // 7. Ӧ�÷�������
    float dot_product = velocity.dot(actual_normal);

    // ֻ������ֱ����ײ����ٶȷ���
    Vector2 tangent_velocity = velocity - actual_normal * dot_product;
    Vector2 normal_velocity = actual_normal * (-dot_product);

    // 8. Ӧ�÷���ϵ��
    const float restitution = 0.95f;
    velocity = tangent_velocity + normal_velocity * restitution;

    // 9. �������Ŷ���ֹ���޴�ֱ����
    if (abs(velocity.x) < 5.0f) {
        float random_angle = (rand() % 10 - 5) * 0.01f; // ��5% �Ƕȱ仯
        Vector2 new_dir(velocity.x * cos(random_angle) - velocity.y * sin(random_angle),
            velocity.x * sin(random_angle) + velocity.y * cos(random_angle));
        velocity = new_dir.normalize() * velocity.length();
    }

    // 10. ������ײ��λ��
    hit_box->set_position(position);
    hurt_box->set_position(position);

    // �������
    printf("��ײ����: (%.2f, %.2f)\n", actual_normal.x, actual_normal.y);
    printf("�����ٶ�: (%.2f, %.2f) -> �����ٶ�: (%.2f, %.2f)\n",
        dot_product, velocity.y, velocity.x, velocity.y);
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
