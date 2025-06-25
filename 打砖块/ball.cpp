#include "ball.h"
#include "character_manager.h"
#include "brick.h"

#include <random>

Ball::Ball() {
    //paddle = CharacterManager::instance()->get_player();
    
    position = { 0, 700 };            // 初始化球的位置
    velocity = { 0, 0 };           // 初始化球的速度
    enable_through_floor = true;
    set_gravity_enabled(false);

    //hit_box->set_size({ 20, 20 });    
    //hit_box->set_layer_src(CollisionLayer::Ball);
    //hit_box->set_layer_dst(CollisionLayer::None);
    //hit_box->set_owner(this);  // 设置碰撞盒所属对象

    hurt_box->set_size({ 20, 20 });     // 设置碰撞盒大小
    hurt_box->set_layer_src(CollisionLayer::Ball);
    hurt_box->set_layer_dst(CollisionLayer::None);
    hurt_box->set_owner(this);
    hurt_box->set_type(CollisionBox::CollisionBoxType::Circle);

    radius = hurt_box->get_size().y / 2;

    // 设置动画参数
    ball_animation.set_interval(0.1f); // 帧间隔时间
    ball_animation.set_loop(true); // 循环播放
    ball_animation.set_anchor_mode(Animation::AnchorMode::Centered);

    // 添加动画帧
    ball_animation.add_frame(ResourcesManager::instance()->find_image("ball"), 1);
    current_animation = &ball_animation;

    timer_last_position.set_one_shot(false);
    timer_last_position.set_wait_time(0.1f);
    timer_last_position.set_on_timeout([&]() {last_position = position; });
}

Ball::~Ball() {

}

Vector2 Ball::get_reflection_vector(const Vector2& incident, const Vector2& normal) const {
    // 反射公式: R = I - 2(I·N)N
    float dot = incident.dot(normal);
    return incident - 2 * dot * normal;
}

// ball.cpp
void Ball::apply_collision_physics(const Vector2& normal) {
    // 1. 恢复系数 (0.95 = 保留95%的能量)
    const float restitution = 0.95f;
    velocity *= restitution;

    // 2. 随机扰动 (±3度防止死循环)
    const float random_angle = (rand() % 6 - 3) * 0.01f;
    velocity = velocity.rotate(random_angle);

    // 3. 速度限制
    const float min_speed = 300.0f;
    if (velocity.length() < min_speed) {
        velocity = velocity.normalize() * min_speed;
    }
}

void Ball::print_collision_debug(const Vector2& original_pos,
    const Vector2& original_vel,
    const CollisionBox::CollisionInfo& info) {
    printf("碰撞细节:\n");
    printf("位置: (%.1f,%.1f)→(%.1f,%.1f)\n",
        original_pos.x, original_pos.y, position.x, position.y);
    printf("速度: (%.1f,%.1f)→(%.1f,%.1f)\n",
        original_vel.x, original_vel.y, velocity.x, velocity.y);
    printf("法线: (%.2f,%.2f) 穿透: %.2f\n",
        info.normal.x, info.normal.y, info.penetration);
}

void Ball::handle_brick_collision(CollisionBox* brick_box, const CollisionBox::CollisionInfo& info) {
    Brick* brick = dynamic_cast<Brick*>(brick_box->get_owner());
    if (!brick) return;

    // 保存原始值用于调试
    Vector2 original_position = position;
    Vector2 original_velocity = velocity;

    // 1. 位置修正 (防止小球卡在砖块内)
    position += info.normal * info.penetration * 1.1f;

    // 2. 边界保护
    const float screen_bottom = getheight();
    if (position.y > screen_bottom - radius) {
        position.y = screen_bottom - radius;
    }

    // 3. 使用反射向量计算反弹
    if (velocity.dot(info.normal) > 0) {
        // 使用统一的反射计算方法
        velocity = get_reflection_vector(velocity, info.normal);

        // 应用物理参数
        apply_collision_physics(info.normal);
    }

    // 调试输出
    print_collision_debug(original_position, original_velocity, info);
}

// 处理平台碰撞
void Ball::handle_paddle_collision(CollisionBox* paddle_box) {
    // 动态计算反弹角度
    float paddle_center = paddle_box->get_position().x;
    float relative_intersect = (position.x - paddle_center) / (paddle_box->get_size().x / 2);
    float bounce_angle = relative_intersect * 45.0f;  // 最大45度

    velocity.x += speed * sin(bounce_angle * 3.14159f / 180.0f);
    velocity.y = -abs(speed * cos(bounce_angle * 3.14159f / 180.0f));  // 确保向上反弹
    printf("Ball 碰撞 Paddle，角度 %.1f 度\n", bounce_angle);
}

void Ball::on_update(float delta) {

    timer_last_position.on_update(delta);

    Character::on_update(delta);
    hit_box->set_position(position);

    // 左边界碰撞
    if (position.x - radius <= 0) {
        position.x = radius; 
        reverse_x();
    }
    // 右边界碰撞
    else if (position.x + radius >= getwidth()) {
        position.x = getwidth() - radius;
        reverse_x();
    }

    // 上边界碰撞 (添加穿透保护)
    if (position.y - radius <= 0) {
        position.y = radius;
        reverse_y();

        // 防止高速穿透
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

    // 只在球未激活时处理发射
    if (msg.message == WM_LBUTTONUP) {
        is_shot_key_down = true;
    }
}
