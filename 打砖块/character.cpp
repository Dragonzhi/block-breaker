#include "character.h"

Character::Character() {
    hit_box = CollisionManager::instance()->create_collision_box();
    hurt_box = CollisionManager::instance()->create_collision_box();

    timer_invulnerable_status.set_wait_time(1.0f);
    timer_invulnerable_status.set_one_shot(true);
    timer_invulnerable_status.set_on_timeout([&]() {
        is_invulnerable = false;
        });

    timer_invulnerable_blink.set_wait_time(0.075f);
    timer_invulnerable_blink.set_one_shot(false);
    timer_invulnerable_blink.set_on_timeout([&]() {
        is_blink_invisible = !is_blink_invisible;
        });
}

Character::~Character() {
    CollisionManager::instance()->destroy_collision_box(hit_box);
    CollisionManager::instance()->destroy_collision_box(hurt_box);
}

void Character::decrease_hp() {
    if (is_invulnerable) return;

    hp -= 1;
    if (hp > 0) {
        make_invulnerable();
    }

    on_hurt();
}


void Character::on_update(float delta) {
    if (hp <= 0)
        velocity.x = 0;
    if (enable_gravity)
        velocity.y += GRAVITY * delta;

    position += velocity * delta;

    if (position.y >= FLOOR_Y) {
        position.y = FLOOR_Y;
        velocity.y = 0;
    }

    if (position.x <= 0) position.x = 0;
    if (position.x >= getwidth()) position.x = (float)getwidth();

    hurt_box->set_position(get_logic_center());

    timer_invulnerable_status.on_update(delta);

    if (is_invulnerable)
        timer_invulnerable_blink.on_update(delta);
}

void Character::on_render() {
    if (is_invulnerable && is_blink_invisible) return;
    // 这里不再需要状态机相关的动画切换
}

void Character::on_hurt() {}

void Character::switch_state(const string& id) {
    // 去掉状态切换逻辑
}

void Character::set_animation(const string& id) {
    // 去掉动画切换逻辑
}