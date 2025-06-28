#pragma once

#include "character.h"
#include <random>

class Paddle : public Character
{
public:
	Paddle();
	~Paddle();
	void on_input(const ExMessage& msg) override;
	void on_update(float delta) override;
	void on_render(const Camera& camera) override;

	bool can_shot() const {
		return has_ball;
	}

	int get_max_hp() const {
		return max_hp;
	}

	void shake() {
		std::mt19937 gen{ std::random_device{}() };
		std::uniform_int_distribution<> dist(-2, 2);
		paddle_animation.set_position(Vector2(position.x + dist(gen), position.y + dist(gen)));
	}

private:
	int mouse_x = 0;
	bool has_ball = true;
	int max_hp = 3;
	Animation paddle_animation;

	bool is_shaking = false;
	Timer timer_shaking;
};

