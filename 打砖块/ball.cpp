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
bool Ball::check_trajectory_collision(const Vector2& start, const Vector2& end, CollisionBox* brick_box, CollisionBox::CollisionInfo& out_info) {
    // 获取砖块的边界
    Vector2 brick_size = brick_box->get_size();
    Vector2 brick_pos = brick_box->get_position();

    float brick_left = brick_pos.x - brick_size.x / 2;
    float brick_right = brick_pos.x + brick_size.x / 2;
    float brick_top = brick_pos.y - brick_size.y / 2;
    float brick_bottom = brick_pos.y + brick_size.y / 2;

    // 获取球的半径
    float radius = this->radius;

    // 扩展砖块边界以考虑球的半径
    float expanded_left = brick_left - radius;
    float expanded_right = brick_right + radius;
    float expanded_top = brick_top - radius;
    float expanded_bottom = brick_bottom + radius;

    // 使用 slab 算法检测射线与扩展后的砖块的碰撞
    Vector2 ray = end - start;
    float length = ray.length();
    if (length < 0.0001f) return false; // 忽略极小的移动

    Vector2 direction = ray.normalize();
    float t_min = 0.0f;
    float t_max = length;

    // 检查 X 轴
    if (std::abs(direction.x) < 0.0001f) {
        // 平行于 X 轴
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

    // 检查 Y 轴
    if (std::abs(direction.y) < 0.0001f) {
        // 平行于 Y 轴
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

    // 如果有碰撞，计算碰撞点和法线
    if (t_min <= length) {
        Vector2 collision_point = start + direction * t_min;

        // 初始化碰撞信息
        out_info.normal = Vector2(0, 0);
        out_info.penetration = radius;

        // 找到最近的边
        float left_dist = std::abs(collision_point.x - brick_left);
        float right_dist = std::abs(collision_point.x - brick_right);
        float top_dist = std::abs(collision_point.y - brick_top);
        float bottom_dist = std::abs(collision_point.y - brick_bottom);

        float min_dist = min_ex({ left_dist, right_dist, top_dist, bottom_dist });

        if (min_dist == left_dist) out_info.normal = Vector2(-1, 0);
        else if (min_dist == right_dist) out_info.normal = Vector2(1, 0);
        else if (min_dist == top_dist) out_info.normal = Vector2(0, -1);
        else out_info.normal = Vector2(0, 1);

        // 计算穿透深度
        out_info.penetration = radius - min_dist;
        out_info.src = this->hurt_box;
        out_info.dst = brick_box;

        return true;
    }

    return false;
}

void Ball::handle_brick_collision(CollisionBox* brick_box, const CollisionBox::CollisionInfo& info) {
    Brick* brick = dynamic_cast<Brick*>(brick_box->get_owner());
    if (!brick) return; // 检查砖块有效性

    // 添加最小穿透阈值
    const float MIN_PENETRATION = 1.0f;
    if (info.penetration < MIN_PENETRATION) {
        return; // 忽略微小穿透
    }
    // 保存修正前位置
    Vector2 original_position = position;

    //// 应用位置修正
    //position += info.normal * info.penetration * 1.1f;

    //// 防止修正后超出屏幕底部
    //const float screen_bottom = getheight();
    //if (position.y > screen_bottom - radius) {
    //    position.y = screen_bottom - radius;
    //}
    //// 使用碰撞法线进行反弹
    //float dot_product = velocity.dot(info.normal);
    //Vector2 tangent_velocity = velocity - dot_product * info.normal;
    //Vector2 normal_velocity = -dot_product * info.normal;

    //// 应用恢复系数
    //const float restitution = 0.95f;
    //if (velocity.x >= 300 && velocity.x >=300) {
    //    velocity = tangent_velocity + normal_velocity * restitution;
    //}
    //else {
    //    velocity = tangent_velocity + normal_velocity;
    //}
        // 位置修正（带阻尼）
    Vector2 correction = info.normal * (info.penetration + 0.5f) * 0.8f;
    position += correction;

    // 速度反射（带随机扰动防止重复碰撞）
    float restitution = 0.9f;
    float dot = velocity.dot(info.normal);
    velocity -= (1.0f + restitution) * dot * info.normal;

    // 添加随机扰动
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.1f, 0.1f);
    velocity.x += dist(gen) * 50.0f;
    velocity.y += dist(gen) * 50.0f;

    // 调试输出
    printf("碰撞修正: (%.1f,%.1f) → (%.1f,%.1f), 法线(%.2f,%.2f), 穿透%.2f\n",
        original_position.x, original_position.y,
        position.x, position.y,
        info.normal.x, info.normal.y, info.penetration);
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

    if (velocity.x > 1000.0f) {
        velocity.x = 1000.0f;
    }
    if (velocity.y > 1000.0f) {
        velocity.y = 1000.0f;
    }

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
