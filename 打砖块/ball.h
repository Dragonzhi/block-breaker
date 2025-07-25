#pragma once

#include "character.h"
#include "collision_box.h"
#include "collision_manager.h"
#include "paddle.h"
#include "resources_manager.h"


class Ball : public Character
{
public:
	Ball(int x, int y, Vector2 velocity, bool is_ready);
	~Ball();

	void on_update(float delta) override;
	void on_input(const ExMessage& msg) override;
	void on_render(const Camera& camera) override;

	void handle_paddle_collision(CollisionBox* paddle_box);
	void handle_brick_collision(CollisionBox* brick_box, const CollisionBox::CollisionInfo& info);
	Vector2 get_reflection_vector(const Vector2& incident, const Vector2& normal) const;
	void apply_collision_physics(const Vector2& normal);

	void print_collision_debug(const Vector2& original_pos,
		const Vector2& original_vel,
		const CollisionBox::CollisionInfo& info);

	void set_paddle(Character* paddle) {
		this->paddle = paddle;
	}

	void set_speed(float val) {
		speed = val;
	}

	float get_speed() const {
		return speed;
	}

	bool check_enable() const {
		return is_enable;
	}

	void reverse_x() {
		velocity.x = -velocity.x;
	}
	void reverse_y() {
		velocity.y = -velocity.y;
	}

	void set_enable(bool flag) {
		is_enable = flag;
	}

	void set_undead(bool flag) {
		is_undead = flag;
	}

	bool get_undead() const {
		return is_undead;
	}

	void to_big() {
		timer_big.restart();
		addition_damage += 1;
		if (addition_damage >= 4) {
			current_animation = &ball_big_ex_animation;
		}
		else {
			current_animation = &ball_big_animation;
		}
		hurt_box->set_size({ 30, 30 });     // ������ײ�д�С
		is_big = true;
	}

	void to_normal() {
		current_animation = &ball_animation;
		hurt_box->set_size({ 20, 20 });     // ������ײ�д�С
		addition_damage = 0;
		is_big = false;
	}

	int get_addition_damage() const {
		return addition_damage;
	}

private:
	Character* paddle = nullptr;
	Timer timer_last_position;
	Timer timer_big;

	Animation ball_animation;
	Animation ball_big_animation;
	Animation ball_big_ex_animation;

	int addition_damage = 0;

	Vector2 last_position;
	bool is_shot_key_down = false;
	float radius;
	bool is_enable = true;
	float speed = 500.0f;
	bool is_ready_shot = true;
	bool is_undead = false;
	bool is_big = false;
};

