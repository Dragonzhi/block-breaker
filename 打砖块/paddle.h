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

private:
	int mouse_x = 0;
	bool has_ball = true;

	Animation paddle_animation;
};

