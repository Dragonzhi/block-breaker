#pragma once

#include "ball.h"
#include "collision_box.h"
#include "collision_manager.h"
#include "game_object.h"
#include "resources_manager.h"

class Brick : public GameObject
{
public:
	enum Type
	{
		Normal,
		Streng,
		Powerup
	};
public:
	Brick(int x, int y, Type type = Type::Normal);

	~Brick();

	void on_update(float delta) override;
	void on_render() override;
	void on_input(const ExMessage& msg) override;
	
	void init();

	void on_hit(); // ש�鱻����ʱ���߼�

	bool can_be_hit() const {
		return cooldown_timer <= 0;
	}

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

	CollisionBox* get_collision_box() const {
		return hurt_box;
	}

	bool check_can_destoryed() const {
		return is_destoryed;
	}

private:
	int counts;					//��Ҫ������Ĵ���
	int points;					//����
	bool is_destoryed = true;		//�Ƿ���Ա��ƻ�
	bool is_active = true;
	Type type;
	CollisionBox* hurt_box;
	float cooldown_timer = 0.0f; // ��ȴ��ʱ�����룩
	const float COOLDOWN_TIME = 0.01f; // ��ȴʱ�䣨�룩
	Animation animation_brick;
};

