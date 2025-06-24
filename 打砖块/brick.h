#pragma once

#include "ball.h"
#include "collision_box.h"
#include "collision_manager.h"
#include "game_object.h"
#include "resources_manager.h"

class Brick : public GameObject
{
private:
	enum Type
	{
		Normal,
		Streng,
		Powerup
	};
public:
	Brick(int x, int y, int cnt = 1, int points = 1, Type type = Type::Normal);

	~Brick();

	void on_update(float delta) override;
	void on_render() override;
	void on_input(const ExMessage& msg) override;
	
	void init();

	int get_points() const {
		return points;
	}

	int get_count() const {
		return counts;
	}

	void set_count(int val) {
		counts = val;
	}

	void set_points(int val) {
		points = val;
	}

	Type get_type() const {
		return type;
	}

	bool check_is_active() const {
		return is_active;
	}

private:
	int counts;					//需要被击打的次数
	int points;					//分数
	bool is_destoryed = true;		//是否可以被破坏
	bool is_active = true;
	Type type;
	CollisionBox* hurt_box;

	Animation animation_brick;
};

