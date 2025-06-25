#pragma once

#include "character.h"
#include "collision_box.h"
#include "collision_manager.h"
#include "paddle.h"
#include "resources_manager.h"


class Ball : public Character
{
public:
	Ball();
	~Ball();

	void on_update(float delta) override;
	void on_input(const ExMessage& msg) override;
	void on_render() override;

	void handle_paddle_collision(CollisionBox* paddle_box);
	void handle_brick_collision(CollisionBox* brick_box, const CollisionBox::CollisionInfo& info);

	void set_paddle(Character* paddle) {
		this->paddle = paddle;
	}

	void set_speed(float val) {
		speed = val;
	}

	float get_speed() const {
		return speed;
	}

	bool cheek_enable() const {
		return is_enable;
	}

	void reverse_x() {
		velocity.x = -velocity.x;
	}
	void reverse_y() {
		velocity.y = -velocity.y;
	}

	bool get_enable() const {
		return is_enable;
	}

	void set_undead(bool flag) {
		is_undead = flag;
	}

	bool get_undead() const {
		return is_undead;
	}

private:
	Character* paddle = nullptr;
	Timer timer_last_position;
	Animation ball_animation;
	Vector2 last_position;
	bool is_shot_key_down = false;
	float radius;
	bool is_enable = false;
	float speed = 400.0f;
	
	bool is_undead = true;
};

