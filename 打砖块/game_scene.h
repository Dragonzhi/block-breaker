#pragma once
#include "scene.h"
#include "button.h"

class GameScene : public Scene
{
public:
	GameScene() ;
	~GameScene() ;

	void on_update(float delta) override ;
	void on_input(const ExMessage& msg) override ;
	void on_enter() override ;
	void on_exit() override ;
	void on_render(const Camera& camera) override ;

	void rest();

	void render_game_overed(const Camera& camera);

	void set_undead() {
		timer_undead.restart();
		is_time_to_undead = true;
	}

private:
	IMAGE* img_background = nullptr;
	IMAGE* img_purple_buff = nullptr;
	int AllPoints = 0;

	bool is_game_overed = false;
	bool is_end_game_bg_ok = false;
	bool is_debug = false;
	bool is_undead_mode = false;

	POINT end_game_bg_position;

	IMAGE* image_end_game_bg = nullptr;
	IMAGE* image_end_game_star_left = nullptr;
	IMAGE* image_end_game_star_centre = nullptr;
	IMAGE* image_end_game_star_right = nullptr;

	Button* button_next = nullptr;
	Button* button_home = nullptr;
	Button* button_rest = nullptr;

	Timer timer_undead;
	bool is_time_to_undead = false;
	
	bool is_cheat = false;
};

