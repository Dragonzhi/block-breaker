#pragma once

#include "character.h"

class Player : public Character
{
public:
	enum class AttackDir
	{
		Up, Down, Left, Right
	};

public:
	Player();
	~Player();

	void on_input(const ExMessage& msg) override;
	void on_update(float delta) override;
	void on_render() override;

	void on_hurt() override;

	void set_rolling(bool flag) {
		is_rolling = flag;
	}

	bool get_rolling() const {
		return is_rolling;
	}

	bool can_roll() const {
		return is_roll_cd_comp && !is_rolling && is_roll_key_down;
	}
	
	void set_attacking(bool flag) {
		is_attacking = flag;
	}

	bool get_attacking() const {
		return is_attacking;
	}

	bool can_attack() const {
		return is_attack_cd_comp && !is_attacking && is_attack_key_down;
	}

	bool can_jump() const {
		return is_on_floor() && is_jump_key_down;
	}

	int get_move_axis() const {
		return is_right_key_down - is_left_key_down;
	}

	AttackDir get_attack_dir() const {
		return attack_dir;
	}

	void on_jump();
	void on_land();
	void on_roll();
	void on_attack();

private:
	void update_attack_dir(int x, int y);

private:
	const float CD_ROLL = 0.75f;
	const float CD_ATTACK = 0.5f;
	const float SPEED_RUN = 300.0f;
	const float SPEED_JUMP = 700.0f;
	const float SPEED_ROLL = 800.0f;

private:
	Timer timer_roll_cd;
	bool is_rolling = false;
	bool is_roll_cd_comp = true;

	Timer timer_attack_cd;
	bool is_attacking = false;
	bool is_attack_cd_comp = true;

	bool is_left_key_down = false;
	bool is_right_key_down = false;
	bool is_jump_key_down = false;
	bool is_roll_key_down = false;
	bool is_attack_key_down = false;

	Animation animation_slash_up;
	Animation animation_slash_down;
	Animation animation_slash_left;
	Animation animation_slash_right;
	AttackDir attack_dir = AttackDir::Right;
	Animation* current_slash_animation = nullptr;

	bool is_jump_vfx_visible = false;
	Animation animation_jump_vfx;

	bool is_land_vfx_visible = false;
	Animation animation_land_vfx;
};

