#pragma once

#include "game_object.h"
#include "collision_box.h"

class Ball : public GameObject
{
public:
	Ball();
	~Ball();

	void on_update(float delta) override;
	void on_input(const ExMessage& msg) override;
	void on_render() override;

private:
	CollisionBox* hurt_box = nullptr;
};

