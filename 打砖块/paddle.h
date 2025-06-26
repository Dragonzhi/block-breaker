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

	int get_max_hp() const {
		return max_hp;
	}

private:
	int mouse_x = 0;
	bool has_ball = true;
	int max_hp = 3;
	Animation paddle_animation;
};

