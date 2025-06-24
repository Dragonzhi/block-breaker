#include "ball.h"
#include "character_manager.h"

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

    // 设置碰撞回调
    hit_box->set_on_collide([this](CollisionBox* other, CollisionBox* self) {
        if (other->get_layer_src() == CollisionLayer::Brick) {
            handle_brick_collision(other);
        }
        });

    hurt_box->set_on_collide([this](CollisionBox* other, CollisionBox* self) {
        if (other->get_layer_src() == CollisionLayer::Paddle) {
            handle_paddle_collision(other);
        }
        });
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
    // 计算碰撞法线（基于相对位置）
    Vector2 brick_center = brick_box->get_position();
    Vector2 normal = { 0, 0 };

    // 判断碰撞方向（左右/上下）
    float overlapX = (hit_box->get_size().x / 2 + brick_box->get_size().x / 2) - abs(position.x - brick_center.x);
    float overlapY = (hit_box->get_size().y / 2 + brick_box->get_size().y / 2) - abs(position.y - brick_center.y);

    // 从最小重叠方向反弹
    if (overlapX < overlapY) {
        normal.x = (position.x < brick_center.x) ? -1 : 1; // 左/右碰撞
    }
    else {
        normal.y = (position.y < brick_center.y) ? -1 : 1; // 上/下碰撞
    }

    // 根据法线反弹
    if (normal.x != 0) velocity.x = -velocity.x;
    if (normal.y != 0) velocity.y = -velocity.y;

    // 防止卡入砖块
    position.x += normal.x * overlapX * 0.5f;
    position.y += normal.y * overlapY * 0.5f;
}

// 处理平台碰撞
void Ball::handle_paddle_collision(CollisionBox* paddle_box) {
    // 动态计算反弹角度
    float paddle_center = paddle_box->get_position().x;
    float relative_intersect = (position.x - paddle_center) / (paddle_box->get_size().x / 2);
    float bounce_angle = relative_intersect * 45.0f;  // 最大45度

    velocity.x = speed * sin(bounce_angle * 3.14159f / 180.0f);
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
