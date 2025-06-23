#pragma once

#include "character.h"

class Paddle : public Character
{
public:
	Paddle();
	~Paddle();
	void on_input(const ExMessage& msg) override;
	void on_update(float delta) override;
	void on_render() override;

	bool can_shot() const {
		return has_ball;
	}

	int get_move_axis() const {
		return is_right_key_down - is_left_key_down;
	}

private:
	float SPEED_MOVE = 400.0f;

	bool is_left_key_down = false;
	bool is_right_key_down = false;
	bool is_shot_key_down = false;

	int mouse_x = 0;
	bool has_ball = true;

	Animation animation_idle_left;
	Animation animation_idle_right;
	Animation animation_move_left;
	Animation animation_move_right;
};

