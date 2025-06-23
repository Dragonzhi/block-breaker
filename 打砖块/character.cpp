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

void Character::on_input(const ExMessage& msg){}

void Character::on_update(float delta) {
	//std::cout << "Delta: " << delta << " seconds" << std::endl;
	state_machine.on_update(delta);

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

	if (!current_animation) return;

	Animation& animation = (is_facing_left ? current_animation->left : current_animation->right);
	animation.on_update(delta);
	animation.set_position(position);
}

void Character::on_render() {
	if (!current_animation || (is_invulnerable && is_blink_invisible)) return;
	(is_facing_left ? current_animation->left : current_animation->right).on_render();
}

void Character::on_hurt() {}

void Character::switch_state(const string& id) {
	state_machine.switch_to(id);
}

void Character::set_animation(const string& id) {
	current_animation = &animation_pool[id];
	current_animation->left.reset();
	current_animation->right.reset();
	//auto& anim = is_facing_left ? current_animation->left : current_animation->right;
	//anim.reset();  // 只重置当前方向的动画
} 