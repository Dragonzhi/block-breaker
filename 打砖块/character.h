#pragma once

#include "vector2.h"
#include "animation.h"
#include "collision_box.h"
#include "game_object.h"

#include <string>
#include <graphics.h>
#include <unordered_map>

class Character : public GameObject
{
public:
    Character();
    ~Character();

    void on_update(float delta) override;
    void on_render() override;
    void on_input(const ExMessage& msg) override;

    void decrease_hp();

    int get_hp() const {
        return hp;
    }

    void set_hp(int val) {
        hp = val;
    }

    void set_position(const Vector2& pos) {
        position = pos;
    }

    const Vector2& get_position() const {
        return position;
    }

    void set_velocity(const Vector2& velocity) {
        this->velocity = velocity;
    }

    const Vector2& get_velocity() const {
        return velocity;
    }

    Vector2 get_logic_center() const {
        return Vector2(position.x, position.y - logic_height / 2);
    }

    void set_gravity_enabled(bool flag) {
        enable_gravity = flag;
    }

    CollisionBox* get_hit_box() {
        return hit_box;
    }

    CollisionBox* get_hurt_box() {
        return hurt_box;
    }

    bool is_on_floor() const {
        return position.y >= FLOOR_Y;
    }

    float get_floor_y() const {
        return FLOOR_Y;
    }

    void make_invulnerable() {
        is_invulnerable = true;
        timer_invulnerable_status.restart();
    }

    virtual void on_hurt();

protected:
    const float FLOOR_Y = 620;                                       // 地板的垂直位置
    const float GRAVITY = 980 * 2;                                   // 重力大小

    int hp = 10;                                                     // 角色生命值
    float logic_height = 0;                                          // 角色逻辑高度
    bool is_facing_left = true;                                      // 当前角色是否面朝左边
    bool enable_gravity = true;                                      // 是否启用重力模式
    bool enable_through_floor = false;                               // 是否穿过地板
    bool is_invulnerable = false;                                    // 当前是否无敌
    Timer timer_invulnerable_blink;                                  // 无敌闪烁状态计时器
    Timer timer_invulnerable_status;                                 // 无敌状态计时器
    bool is_blink_invisible = false;                                 // 当前是否处于闪烁的不可见帧
    CollisionBox* hit_box = nullptr;                                 // 攻击碰撞盒
    CollisionBox* hurt_box = nullptr;                                // 伤害碰撞盒
    Animation* current_animation = nullptr;                          // 当前角色动画
    std::unordered_map<std::string, Animation> animation_pool;       // 角色动画池
};