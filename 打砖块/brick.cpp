#include "brick.h"
#include "camera.h"
#include "character_manager.h"
#include "sound_manager.h"
#include "scene_manager.h"
#include "game_scene.h"
#include <iostream>

Brick::Brick(int x, int y, Type type) {
    position.x = x;
    position.y = y;
    this->type = type;

    animation_brick.set_interval(0.1f);
    animation_brick.set_loop(true);
    animation_brick.set_anchor_mode(Animation::AnchorMode::Centered);

    hurt_box = CollisionManager::instance()->create_collision_box();
    hurt_box->set_owner(this);
    hurt_box->set_size({ 60, 30 });     // 设置碰撞盒大小
    hurt_box->set_layer_src(CollisionLayer::Brick);
    hurt_box->set_layer_dst(CollisionLayer::Ball);
    hurt_box->set_on_collide([this](CollisionBox* src, CollisionBox* dst, const CollisionBox::CollisionInfo& info) {
        if (src && src->get_layer_src() == CollisionLayer::Ball ) {
            // 球的反弹逻辑（可根据需要实现）
            Ball* ball = dynamic_cast<Ball*>(src->get_owner());
            if (ball && this->type != Brick::Type::Glass && !(ball->get_addition_damage() >= 4)) {
                ball->handle_brick_collision(this->hurt_box, info);
            }

            on_hit(ball);

        }
        });

    size.x = hurt_box->get_size().x;
    size.y = hurt_box->get_size().y;

    timer_shake.set_one_shot(true);
    timer_shake.set_wait_time(0.1f);
    timer_shake.set_on_timeout([this]() {is_shake = false; });

    init();
}

Brick::~Brick() {
    CollisionManager::instance()->destroy_collision_box(hurt_box);
}

void Brick::on_hit(Ball* ball) {
    if (!is_active || !can_be_hit()) return; // 双重检查
    is_shake = true;
    if (check_can_destoryed()) {
        counts -= 1 + ball->get_addition_damage();
    }
    cooldown_timer = COOLDOWN_TIME; // 重置冷却时间
    
    SoundManager::instance()->playSound(_T("ball_brick"), false);
    // 日志输出剩余击中次数
    std::cout << "Brick被击中，剩余击中次数: " << counts << std::endl;
    generate_particles(10, color.r, color.g, color.b, 255, false);
    if (counts <= 0) {
        // 砖块被摧毁的逻辑
        std::cout << "Brick被摧毁" << std::endl;
        SoundManager::instance()->playSound(_T("brick_broken"), false);
        is_active = false;
        Camera::instance()->shake(rand() % 3 + 1, 0.1);
        generate_particles(20, color.r, color.g, color.b, 255, true);

        if (type == Brick::Double) {
            Vector2 temp_velocity = ball->get_velocity();
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(-70, 70);
            temp_velocity.x += dist(gen);
            temp_velocity.y += dist(gen);
            CharacterManager::instance()->add_ball(position.x, position.y, temp_velocity, false);
        }
        else if (type == Brick::Streng) {
            Paddle* paddle = (Paddle*)CharacterManager::instance()->get_player();
            paddle->to_big();
        }
        else if (type == Brick::Powerup) {
            ball->to_big();
        }
        else if (type == Brick::More) {
            GameScene* game_scene = (GameScene*)SceneManager::instance()->get_current_scene();
            game_scene->set_undead();
        }
    }
}

void Brick::generate_particles(int num, int r, int g, int b, int a, bool is_blink = false) {
    std::random_device rd;
    std::mt19937 gen(rd());
    // 产生粒子效果
    for (int i = 0; i < num; ++i) {
        std::uniform_int_distribution<> dist(-30, 30);
        Vector2 position(this->position.x + dist(gen), this->position.y + dist(gen) / 2);
        Vector2 velocity((-50 + rand() % 100) / 2, (rand() % 100));
        float life_time = (rand() % 10 + 2) / 10.0f;
        ParticleSystem::instance()->add_particle(position, velocity, life_time, r, g, b, a, is_blink);
    }
}

void Brick::on_update(float delta) {
    if (!is_active) return;

    if (cooldown_timer > 0) {
        cooldown_timer -= delta;
        hurt_box->set_enabled(false);
    }
    else {
        hurt_box->set_enabled(true);
    }

    if (is_shake) {
        timer_shake.on_update(delta);
    }

    hurt_box->set_position(Vector2(position.x, position.y));

    animation_brick.on_update(delta);
    animation_brick.set_position(position);

    if (is_shake) {
        make_shake();
    }
}

void Brick::on_render(const Camera& camera) {
    animation_brick.on_render(camera);
}

void Brick::init() {
    switch (type)
    {
    case Brick::Normal:
        animation_brick.add_frame(ResourcesManager::instance()->find_image("brick_blue"), 1);
        counts = 1;
        points = 1;
        color.r = 74;
        color.g = 190;
        color.b = 240;
        break;
    case Brick::Streng:
        animation_brick.add_frame(ResourcesManager::instance()->find_image("brick_red"), 1);
        counts = 2;
        points = 2;
        color.r = 240;
        color.g = 50;
        color.b = 50;
        break;
    case Brick::Powerup:
        animation_brick.add_frame(ResourcesManager::instance()->find_image("brick_yellow"), 1);
        counts = 3;
        points = 3;
        color.r = 255;
        color.g = 205;
        color.b = 0;
        break;
    case Brick::Double:
        animation_brick.add_frame(ResourcesManager::instance()->find_image("brick_green"), 1);
        counts = 1;
        points = 2;
        color.r = 160;
        color.g = 205;
        color.b = 50;
        break;
    case Brick::More:
        animation_brick.add_frame(ResourcesManager::instance()->find_image("brick_purple"), 1);
        counts = 5;
        points = 5;
        color.r = 140;
        color.g = 85;
        color.b = 155;
        break;
    case Brick::Glass:
        animation_brick.add_frame(ResourcesManager::instance()->find_image("brick_grey"), 1);
        counts = 1;
        points = 3;
        color.r = 205;
        color.g = 205;
        color.b = 205;
        break;
    case Brick::Unbreakable:
        animation_brick.add_frame(ResourcesManager::instance()->find_image("brick_unbreakable"), 1);
        is_destoryed = false; // 不可被击中
        counts = 1;
        points = 0;
        color.r = 113;
        color.g = 113;
        color.b = 113;
    default:
        break;
    }
}

void Brick::on_input(const ExMessage& msg) {}