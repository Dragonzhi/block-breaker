#pragma once

#include "character.h"
#include "collision_box.h"
#include "collision_manager.h"
#include "paddle.h"

class Ball : public Character
{
public:
	Ball();
	~Ball();

	void on_update(float delta) override;
	void on_input(const ExMessage& msg) override;
	void on_render() override;

	void set_paddle(Paddle* paddle) {
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

private:
	CollisionBox* hurt_box = nullptr;
	Paddle* paddle = nullptr;

	bool is_enable = true;
	float speed = 200.0f;

};

