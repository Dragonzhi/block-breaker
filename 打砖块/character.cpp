#include "character.h"
#include "collision_manager.h"

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

void Character::on_input(const ExMessage& msg) {}

void Character::on_update(float delta) {
    if (hp <= 0)
        velocity.x = 0;
    if (enable_gravity)
        velocity.y += GRAVITY * delta;

    position += velocity * delta;

    if (position.y >= FLOOR_Y && !enable_through_floor) {
        position.y = FLOOR_Y;
        velocity.y = 0;
    }

    if (position.x <= 0) position.x = 0;
    if (position.x >= getwidth()) position.x = (float)getwidth();

    hurt_box->set_position(get_logic_center());

    if (!current_animation) return;

    current_animation->on_update(delta);
    current_animation->set_position(position);
}

void Character::on_render(const Camera& camera) {
    current_animation->on_render(camera);
}

void Character::on_hurt() {}