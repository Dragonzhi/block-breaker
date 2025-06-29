#pragma once

#include "ball.h"
#include "collision_box.h"
#include "collision_manager.h"
#include "game_object.h"
#include "particle_system.h"
#include "resources_manager.h"
#include <random>

class Brick : public GameObject
{
public:
    enum Type
    {
        Normal,
        Streng,
        Powerup,
        Double,
        More,
        Glass
    };
    struct ColorBrick
    {
        int r;
        int g;
        int b;
    };
public:
    Brick(int x, int y, Type type = Type::Normal);
    ~Brick();

    void on_update(float delta) override;
    void on_render(const Camera& camera) override;
    void on_input(const ExMessage& msg) override;

    void init();
    void on_hit(Ball* ball);        // 砖块被击中时的逻辑
    void generate_particles(int num, int r, int g, int b, int a, bool is_blink);


    bool can_be_hit() const {
        return cooldown_timer <= 0;
    }

    int get_points() const {
        return points;
    }

    int get_count() const {
        return counts;
    }

    void set_count(int val) {
        counts = val;
    }

    void set_points(int val) {
        points = val;
    }

    Type get_type() const {
        return type;
    }

    bool check_is_active() const {
        return is_active;
    }

    CollisionBox* get_collision_box() const {
        return hurt_box;
    }

    bool check_can_destoryed() const {
        return is_destoryed;
    }

    void make_shake() {
        std::mt19937 gen{ std::random_device{}() };
        std::uniform_int_distribution<> dist(-be_hit_range, be_hit_range);
        animation_brick.set_position(Vector2(position.x + dist(gen), position.y + dist(gen)));
    }

private:
    int counts = 1;                   // 需要击中的次数
    int points = 1;                   // 得分
    bool is_destoryed = true;         // 是否可以被摧毁
    bool is_active = true;

    Vector2 size;
    ColorBrick color;

    Type type;
    CollisionBox* hurt_box = nullptr;
    float cooldown_timer = 0.0f;      // 冷却时间计数器（秒）
    const float COOLDOWN_TIME = 0.03f; // 冷却时间（秒）
    Animation animation_brick;

    bool is_shake = false;      // 无敌状态
    Timer timer_shake;          // 无敌状态计时器

    int be_hit_range = 2;
};