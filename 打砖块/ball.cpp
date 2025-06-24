#include "ball.h"
#include "character_manager.h"
#include "brick.h"

Ball::Ball() {
    //paddle = CharacterManager::instance()->get_player();
    
    position = { 0, 700 };            // 初始化球的位置
    velocity = { 0, 0 };           // 初始化球的速度
    enable_through_floor = true;
    set_gravity_enabled(false);

    hit_box->set_size({ 20, 20 });    
    hit_box->set_layer_src(CollisionLayer::Ball);
    hit_box->set_layer_dst(CollisionLayer::None);

    hurt_box->set_size({ 20, 20 });     // 设置碰撞盒大小
    hurt_box->set_layer_src(CollisionLayer::Ball);
    hurt_box->set_layer_dst(CollisionLayer::None);

    hit_box->set_owner(this);  // 设置碰撞盒所属对象
    hurt_box->set_owner(this);

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


// 处理砖块碰撞
void Ball::handle_brick_collision(CollisionBox* brick_box) {
    printf("Ball处理砖块碰撞！坐标(%.1f,%.1f)\n", position.x, position.y);
    // 获取砖块精确信息
    Brick* brick = dynamic_cast<Brick*>(brick_box->get_owner());
    if (!brick) return;

    Vector2 brick_center = brick->get_position();
    Vector2 brick_size = brick_box->get_size();

    // 计算球的半径（假设球是圆形）
    float ball_radius = hit_box->get_size().x / 2.0f;

    // 1. 计算AABB与圆形的碰撞
    Vector2 closest_point;
    closest_point.x =  max(brick_center.x - brick_size.x / 2,
         min(position.x, brick_center.x + brick_size.x / 2));
    closest_point.y =  max(brick_center.y - brick_size.y / 2,
         min(position.y, brick_center.y + brick_size.y / 2));

    // 2. 计算球心到最近点的向量
    Vector2 collision_vector = position - closest_point;
    float distance = collision_vector.length();

    // 3. 检查是否发生碰撞
    if (distance > ball_radius) {
        return; // 没有碰撞
    }

    // 4. 计算最小位移向量(MTV)
    Vector2 collision_normal = collision_vector.normalize();
    float penetration_depth = ball_radius - distance;

    // 5. 位置修正：将球推出碰撞区域
    position += collision_normal * penetration_depth * 1.1f;

    // 6. 计算碰撞法线（基于碰撞面）
    Vector2 actual_normal = { 0, 0 };

    // 确定碰撞发生在哪一侧
    Vector2 to_center = brick_center - position;
    float abs_x = abs(to_center.x);
    float abs_y = abs(to_center.y);

    // 计算重叠量
    float overlap_x = ball_radius + brick_size.x / 2 - abs_x;
    float overlap_y = ball_radius + brick_size.y / 2 - abs_y;

    // 确定最小重叠方向（真正的碰撞法线）
    if (overlap_x < overlap_y) {
        actual_normal.x = (to_center.x < 0) ? -1 : 1;
    }
    else {
        actual_normal.y = (to_center.y < 0) ? -1 : 1;
    }

    // 7. 应用反弹物理
    float dot_product = velocity.dot(actual_normal);

    // 只反弹垂直于碰撞面的速度分量
    Vector2 tangent_velocity = velocity - actual_normal * dot_product;
    Vector2 normal_velocity = actual_normal * (-dot_product);

    // 8. 应用反弹系数
    const float restitution = 0.95f;
    velocity = tangent_velocity + normal_velocity * restitution;

    // 9. 添加随机扰动防止无限垂直弹跳
    if (abs(velocity.x) < 5.0f) {
        float random_angle = (rand() % 10 - 5) * 0.01f; // ±5% 角度变化
        Vector2 new_dir(velocity.x * cos(random_angle) - velocity.y * sin(random_angle),
            velocity.x * sin(random_angle) + velocity.y * cos(random_angle));
        velocity = new_dir.normalize() * velocity.length();
    }

    // 10. 更新碰撞盒位置
    hit_box->set_position(position);
    hurt_box->set_position(position);

    // 调试输出
    printf("碰撞法线: (%.2f, %.2f)\n", actual_normal.x, actual_normal.y);
    printf("入射速度: (%.2f, %.2f) -> 反射速度: (%.2f, %.2f)\n",
        dot_product, velocity.y, velocity.x, velocity.y);
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

    // 只在球未激活时处理发射
    if (msg.message == WM_LBUTTONDOWN) {
        is_shot_key_down = true;
    }
}
